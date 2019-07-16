#include "data.h"

#include "utils.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

namespace pa {
constexpr char const * const msgFinishedReading("Reading has been finished");

DataFile::~DataFile() = default;
IData::~IData() = default;

//TODO: add logging to file
void ChildrenNames::read(std::string_view fileName)
{
    std::ifstream ifs(fileName.data(), std::ios_base::in);
    m_fileName = fileName;
    if (!ifs.is_open())
        throw std::invalid_argument("Error getting file " + m_fileName);

    utils::printArgs("Reading data from ", m_fileName);
    std::string line;
    while (std::getline(ifs, line))
    {
        ++m_countLines;
        //skip empty strings
        if (line.empty())
        {
            //TODO: add loggin to file, may be using option?
            //utils::printArgs(m_countLines, " is empty");
            continue;
        }

        if (bool inserted = !m_childrenNames.insert(line).second)
        {
            //utils::printArgs(m_countLines, " duplicated data : ", line);
        }
    }

    utils::printArgs(utils::newLine, msgFinishedReading, utils::newLine);
}

void ChildrenRelations::read(std::string_view fileName)
{
    std::ifstream ifs(fileName.data(), std::ios_base::in);
    m_fileName = fileName;
    if (!ifs.is_open())
        throw std::invalid_argument("Error getting file: " + m_fileName);

    utils::printArgs("Reading data from ", m_fileName);

    std::string line;
    std::istringstream iss(line);
    while (std::getline(ifs, line))
    {
        ++m_countLines;
        if (line.empty())
        {
            //TODO: replace with logging
            //utils::printArgs(m_countLines, " is empty");
            continue;
        }

        iss.clear();
        iss.str(line);
        std::string word1, word2;
        if (!(iss >> word1 >> word2))
        {
            //utils::printArgs(m_countLines, " has invalid data: ", line);
            continue;
        }

        if (word1 == word2)
        {
            //utils::printArgs(m_countLines, " has same words: ", line);
            continue;
        }

        m_name2Relations[word1].insert(word2);
    }

    utils::printArgs(utils::newLine, msgFinishedReading, utils::newLine);
}

ProcessDataFacade::ProcessDataFacade(int argc, char ** argv)
{
    if (argc != 3)
        throw std::invalid_argument("You should input 2 files");

    namespace fs = std::filesystem;
    auto wrongFilePath = [](std::string_view str)
    {
        fs::path filePath(str);
        return !fs::exists(filePath) || !filePath.has_filename();
    };
    std::string_view childrenFilePath = argv[1];
    if (wrongFilePath(childrenFilePath))
        throw std::invalid_argument(childrenFilePath.data());

    std::string_view childrenRelationsFilePath = argv[2];
    if (wrongFilePath(argv[2]))
        throw std::invalid_argument(childrenRelationsFilePath.data());

    //TODO: try to parallel task
    m_childrenNames.read(childrenFilePath);
    m_childrenRelations.read(childrenRelationsFilePath);
}

StringList ProcessDataFacade::unlovedChildrenNames() const
{
    auto const & chilrenNames   = m_childrenNames.childrenNames();
    auto const & name2Relations = m_childrenRelations.name2Relations();
    auto foundHappyName = [&name2Relations](auto const & childrenName)
    {
        for (auto const & [dummy, name2Relation] : name2Relations)
            if (name2Relation.find(childrenName) != name2Relation.cend())
                return true;
        return false;
    };
    StringList result;
    for (auto const & childrenName : chilrenNames)
    {
        if (!foundHappyName(childrenName))
            result.push_front(childrenName);
    }
    return result;
}

StringList ProcessDataFacade::unhappyChildrenNames() const
{
    auto const & name2Relations = m_childrenRelations.name2Relations();
    auto foundHappyName = [&name2Relations](auto const & childrenName)
    {
        for (auto const & [dummy, name2Relation] : name2Relations)
            if (name2Relation.find(childrenName) != name2Relation.cend())
                return true;
        return false;
    };
    StringList results;
    for (auto const & [childrenName, dummy] : name2Relations)
    {
        if (!foundHappyName(childrenName))
            results.push_front(childrenName);
    }
    return results;
}

StringList ProcessDataFacade::favouriteChildrenNames() const
{
    //TODO: improve name of variables
    auto const & name2Relations = m_childrenRelations.name2Relations();
    std::unordered_map<std::string, size_t> nameToCount;
    for (auto const & [dummy, name2Relation] : name2Relations)
    {
        for (auto const & nameRelation : name2Relation)
            ++nameToCount[nameRelation];
    }
    size_t const countFilter = 1;
    StringList results;
    for (auto const & [name, count] : nameToCount)
    {
        if (count > countFilter)
            results.push_front(name + ": " + std::to_string(count));
    }
    return results;
}

void ProcessDataFacade::run()
{
    enum class eUserSelect : char
    {
        eExit,
        eUnlovedChildrenNames,
        eUnhappyChildrenNames,
        eFavouriteChildrenNames,
        };

    const std::string_view menu = R"(
Select action:"
    "1 - unloved children"
    "2 - unhappy children"
    "3 - favorite children"
    "------------------------"
    "0 - exit"
===> )";

    bool readAgain = true;
    do
    {
        utils::printArgs(menu);
        int num = 0;
        std::cin >> num;
        if (!std::cin)
        {
            //TODO: improve clearing
            std::cin.clear();
            std::string s;
            std::cin >> s;
            num = -1;
        }
        switch (static_cast<eUserSelect>(num))
        {
        case eUserSelect::eUnlovedChildrenNames:
            utils::printContainerData(unlovedChildrenNames());
            break;
        case eUserSelect::eUnhappyChildrenNames:
            utils::printContainerData(unhappyChildrenNames());
            break;
        case eUserSelect::eFavouriteChildrenNames:
            utils::printContainerData(favouriteChildrenNames());
            break;
        case eUserSelect::eExit:
            utils::printArgs("Bye-bye :)");
            readAgain = false;
            break;
        default:
            utils::printArgs("You entered not existed action, please, try again :)");
            break;
        }
    } while (readAgain);
}
};//end namespace PaveA
