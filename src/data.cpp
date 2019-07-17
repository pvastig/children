#include "data.h"

#include "utils.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

namespace pa {
DataFile::~DataFile() = default;

//TODO: add logging to file
void ChildrenNames::read(std::string_view fileName)
{
    std::ifstream ifs(fileName.data(), std::ios_base::in);
    m_fileName = fileName;
    if (!ifs.is_open())
        throw std::invalid_argument("Error getting file " + m_fileName);

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

        if (auto [it, inserted] = m_childrenNames.insert(line);!inserted)
        {
            //utils::printArgs(m_countLines, " duplicated data : ", line);
        }
    }
}

void ChildrenRelations::read(std::string_view fileName)
{
    std::ifstream ifs(fileName.data(), std::ios_base::in);
    m_fileName = fileName;
    if (!ifs.is_open())
        throw std::invalid_argument("Error getting file: " + m_fileName);

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

        m_name2RelatedNames[word1].insert(word2);
    }
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
    if (wrongFilePath(childrenRelationsFilePath))
        throw std::invalid_argument(childrenRelationsFilePath.data());

    //TODO: try to parallel task
    m_childrenNames.read(childrenFilePath);
    m_childrenRelations.read(childrenRelationsFilePath);
}

StringList ProcessDataFacade::unlovedChildrenNames() const
{
    auto const & chilrenNames   = m_childrenNames.childrenNames();
    auto const & name2Relations = m_childrenRelations.name2RelatedNames();
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
    auto const & name2Relations = m_childrenRelations.name2RelatedNames();
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
    auto const & name2RelatedNames = m_childrenRelations.name2RelatedNames();
    std::unordered_map<std::string, size_t> favoriteName2Count;
    for (auto const & [dummy, relatedNames] : name2RelatedNames)
    {
        for (auto const & relatedName : relatedNames)
            ++favoriteName2Count[relatedName];
    }
    size_t const filter = 1;
    StringList results;
    for (auto const & [name, count] : favoriteName2Count)
    {
        if (count > filter)
            results.push_front(name + ": " + std::to_string(count));
    }
    return results;
}

void ProcessDataFacade::run() const
{
    enum class UserSelect : char
    {
        Exit,
        UnlovedChildrenNames,
        UnhappyChildrenNames,
        FavouriteChildrenNames
    };

    const std::string_view menu = R"(
Select action:"
    "1 - Unloved children"
    "2 - Unhappy children"
    "3 - Favorite children"
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
        switch (static_cast<UserSelect>(num))
        {
        case UserSelect::UnlovedChildrenNames:
            utils::printContainerData(unlovedChildrenNames());
            break;
        case UserSelect::UnhappyChildrenNames:
            utils::printContainerData(unhappyChildrenNames());
            break;
        case UserSelect::FavouriteChildrenNames:
            utils::printContainerData(favouriteChildrenNames());
            break;
        case UserSelect::Exit:
            utils::printArgs("Bye-bye :)", utils::newLine);
            readAgain = false;
            break;
        default:
            utils::printArgs("You entered not existed action, please, try again :)", utils::newLine);
            break;
        }
    } while (readAgain);
}
};//end namespace PaveA
