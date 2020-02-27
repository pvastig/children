#include "data.h"

#include "utils.h"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <future>
#include <vector>

namespace pa {

DataFile::DataFile(std::string_view fileName) : m_fileName(fileName) {}

void DataFile::logWarnings()
{
    if (m_warnings.empty())
        return;

    utils::Log logToFile(m_fileName.data());
    for (auto const& warning : m_warnings)
        logToFile << warning;
}

ChildrenNamesFile::ChildrenNamesFile(std::string_view fileName) : DataFile(fileName) {}

bool ChildrenNamesFile::read()
{
    // TODO: take into account binary mode
    std::ifstream ifs(m_fileName.data());
    assert(ifs.is_open());
    if (!ifs.is_open())
        return false;

    // TODO: add checking if line contains 2 or more words
    std::string line;
    // TODO: improve reading using chunks, take a look at boost?
    WarningVector warnings;
    while (std::getline(ifs, line)) {
        ++m_countLines;
        if (line.empty()) {
            warnings.emplace_back(std::to_string(m_countLines) + " is empty");
            continue;
        }

        if (auto [it, inserted] = m_childrenNames.insert(line); !inserted)
            warnings.emplace_back(std::to_string(m_countLines) + " has duplicated data : " + line);
    }
    m_warnings = std::move(warnings);
    return m_warnings.empty();
}

ChildrenRelationsFile::ChildrenRelationsFile(std::string_view fileName) : DataFile(fileName) {}

bool ChildrenRelationsFile::read()
{
    std::ifstream ifs(m_fileName.data());
    assert(ifs.is_open());
    if (!ifs.is_open())
        return false;

    std::string line;
    WarningVector warnings;
    while (std::getline(ifs, line)) {
        ++m_countLines;
        if (line.empty()) {
            warnings.emplace_back(std::to_string(m_countLines) + " is empty");
            continue;
        }

        // TODO: add check if line contains one word or more than 2 words
        std::istringstream iss(line);
        std::string word1, word2;
        // TODO: investigate this case
        if (!(iss >> word1 >> word2)) {
            warnings.emplace_back(std::to_string(m_countLines) + " has invalid data: " + line);
            continue;
        }
        // The case is from task
        if (word1 == word2) {
            warnings.push_back(std::to_string(m_countLines) + " has same words: " + line);
            continue;
        }
        m_name2RelatedNames[word1].insert(word2);
    }
    m_warnings = std::move(warnings);
    return m_warnings.empty();
}

ProcessDataFacade::ProcessDataFacade(int argc, char const** argv)
{
    if (argc == 4) {
        // TODO: make possibility process arguments and make help for them
        if (argv[3] == std::string("--log")) {
            m_logToFile = true;
        }
    } else if (argc != 3)
        throw std::invalid_argument("You should input 2 files");

    namespace fs = std::filesystem;
    auto wrongFilePath = [](std::string_view str) {
        fs::path filePath(str);
        return !fs::exists(filePath) || !filePath.has_filename();
    };
    std::string_view childrenFilePath = argv[1];
    if (wrongFilePath(childrenFilePath))
        throw std::invalid_argument(childrenFilePath.data() + std::string(" does not exist"));

    std::string_view childrenRelationsFilePath = argv[2];
    if (wrongFilePath(childrenRelationsFilePath))
        throw std::invalid_argument(childrenRelationsFilePath.data()
                                    + std::string(" does not exist"));

    m_dataFile.emplace_back(std::make_unique<ChildrenNamesFile>(childrenFilePath));
    m_dataFile.emplace_back(std::make_unique<ChildrenRelationsFile>(childrenRelationsFilePath));
}

void ProcessDataFacade::run() const
{
    auto const readRes = readData();
    auto parsedResult = ParseResult(readRes).parse();
    assert(parsedResult.size == 2);
    DisplayData(parsedResult).run();
}

ResultVector ProcessDataFacade::readData() const
{
    ResultVector result;
    for (auto&& item : m_dataFile) {
        utils::runAsync([&item]() { item->read(); }).get();
        result.emplace_back(item->result());
    }
    if (m_logToFile) {
        for (auto&& item : m_dataFile)
            item->logWarnings();
    }
    return result;
}

DisplayData::DisplayData(ParsedResult const& result) : m_result(result) {}

void DisplayData::run() const
{
    std::string_view menu = R"(
    Select action:
    1 - Unloved children
    2 - Unhappy children
    3 - Favorite children
    ----------------------
    0 - exit
==> )";

    enum class UserSelect : char {
        Exit,
        UnlovedChildrenNames,
        UnhappyChildrenNames,
        FavouriteChildrenNames
    };
    bool readAgain = true;
    do {
        std::cout << menu;
        int num = -1;
        std::cin >> num;
        if (!std::cin) {
            std::cin.clear();
            std::string s;
            std::cin >> s;
        }
        switch (static_cast<UserSelect>(num)) {
        case UserSelect::UnlovedChildrenNames:
            // TODO: instead of printing to console, print to file?
            std::cout << unlovedChildrenNames(m_result.childrenNames, m_result.name2RelatedNames);
            break;
        case UserSelect::UnhappyChildrenNames:
            std::cout << unhappyChildrenNames(m_result.name2RelatedNames);
            break;
        case UserSelect::FavouriteChildrenNames:
            std::cout << favoriteChildrenNames(m_result.name2RelatedNames);
            break;
        case UserSelect::Exit:
            std::cout << "Bye-bye :)" << utils::newLine;
            readAgain = false;
            break;
        default:
            std::cout << "You entered not existed action, please, try again:)" << std::endl;
            break;
        }
    } while (readAgain);
}

StringList unlovedChildrenNames(StringUnordSet const& childrenNames,
                                StringUnordMap const& name2RelatedNames)
{
    auto foundHappyName = [&name2RelatedNames](auto const& childrenName) {
        for (auto const& [dummy, name2Relation] : name2RelatedNames)
            if (name2Relation.find(childrenName) != name2Relation.cend())
                return true;
        return false;
    };
    StringList result;
    for (auto const& childrenName : childrenNames)
        if (!foundHappyName(childrenName))
            result.push_front(childrenName);
    return result;
}

StringList unhappyChildrenNames(StringUnordMap const& name2RelatedNames)
{
    auto foundHappyName = [&name2RelatedNames](auto const& childrenName) {
        for (auto const& [dummy, name2Relation] : name2RelatedNames)
            if (name2Relation.find(childrenName) != name2Relation.cend())
                return true;
        return false;
    };
    StringList results;
    for (auto const& [childrenName, dummy] : name2RelatedNames)
        if (!foundHappyName(childrenName))
            results.push_front(childrenName);
    return results;
}

StringList favoriteChildrenNames(StringUnordMap const& name2RelatedNames)
{
    std::unordered_map<std::string, size_t> favoriteName2Count;
    for (auto const& [dummy, relatedNames] : name2RelatedNames)
        for (auto const& relatedName : relatedNames)
            ++favoriteName2Count[relatedName];

    StringList results;
    for (auto const& [name, count] : favoriteName2Count)
        if (size_t const filter = 1; count > filter)
            results.push_front(name + ": " + std::to_string(count));
    return results;
}

std::ostream& operator<<(std::ostream& os, StringList const& container)
{
    os << "(\n";
    // TODO: thinking about setting width output
    utils::print(os, container);
    return os << ")";
}

ParseResult::ParseResult(const ResultVector& result) : m_result(result) {}

template<class T>
struct always_false : std::false_type
{};

ParsedResult ParseResult::parse()
{
    // TODO: if size is more 2, it needs to rework the place
    assert(m_result.size() > 2);
    ParsedResult parsedResutl;
    for (auto const& item : m_result) {
        std::visit(
            [&parsedResutl, item](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, StringUnordSet>)
                    parsedResutl.childrenNames = arg;
                else if constexpr (std::is_same_v<T, StringUnordMap>)
                    parsedResutl.name2RelatedNames = arg;
                else
                    static_assert(always_false<T>::value, "non-exhaustive visitor!");
            },
            item);
    }
    return parsedResutl;
}

} // namespace pa
