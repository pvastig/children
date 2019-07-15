#include "data.h"

#include "utils.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace pa
{
constexpr char const * const msgFinishedReading("Reading has been finished");
constexpr char const * const msgPintedFileData("**************** Printed data from file ");

//TODO: add logging to file
void ChildrenNames::read(std::string_view fileName)
{
    std::ifstream ifs(fileName.data(), std::ios_base::in);
    m_fileName = fileName;
    if (!ifs.is_open())
        throw std::invalid_argument("Error getting file: " + m_fileName);

    utils::printArgs("Reading data from ", m_fileName);
    std::string line;
    while (std::getline(ifs, line))
    {
        ++m_countLines;
        //skip empty strings
        if (line.empty())
        {
            utils::printArgs(m_countLines, " is empty");
            continue;
        }

        if (bool inserted = !m_childrenNames.insert(line).second)
            utils::printArgs(m_countLines, " duplicated data : ", line);
    }

    utils::printArgs(msgFinishedReading);
}

void ChildrenNames::print() const
{
    utils::printArgs(msgPintedFileData, m_fileName);
    utils::printContainerData(m_childrenNames);
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
            utils::printArgs(m_countLines, " is empty");
            continue;
        }

        iss.clear();
        iss.str(line);
        std::string word1, word2;
        if (!(iss >> word1 >> word2))
        {
            utils::printArgs(m_countLines, " has invalid data: ", line);
            continue;
        }

        if (word1 == word2)
        {
            utils::printArgs(m_countLines, " has same words: ", line);
            continue;
        }

        m_name2Relations[word1].insert(word2);
    }

    utils::printArgs(msgFinishedReading);
}

void ChildrenRelations::print() const
{
    utils::printArgs(msgPintedFileData, m_fileName);
    for (auto const & nameRelation : m_name2Relations)
    {
        const auto& names = nameRelation.second;
        if (names.empty())
            continue;

        const auto& firstName = nameRelation.first;
        const auto& firstRelationName = names.begin();
        utils::printArgs(firstName, '\t', *firstRelationName, utils::newLine);
        std::for_each(std::next(names.begin()), names.end(),
                      [](const auto& relationName)
                      {
                          utils::printArgs('\t', relationName, utils::newLine);
                      }
                      );
    }
}

ProcessDataFacade::ProcessDataFacade(int const argc, char ** argv)
    : m_argc(argc),
      m_argv(argv)
{
    if (argc != 3)
        throw std::invalid_argument("You should enter input files");

    //TODO: move to other method
    m_childrenNames.read(argv[1]);
    m_childrenRelations.read(argv[2]);
}

StringList ProcessDataFacade::unlovedChildrenNames() const
{
    auto const & chilrenNames   = m_childrenNames.childrenNames();
    auto const & name2Relations = m_childrenRelations.name2Relations();
    StringList results;
    //TODO: improve loop
    for (auto const & childrenName : chilrenNames)
    {
        bool foundHappyName = false;
        for (auto const & [dummy, name2Relation] : name2Relations)
        {
            if (name2Relation.find(childrenName) != name2Relation.cend())
            {
                foundHappyName = true;
                break;
            }
        }

        if (!foundHappyName)
            results.push_front(childrenName);
    }

    return results;
}

StringList ProcessDataFacade::unhappyChildrenNames() const
{
    auto const & name2Relations = m_childrenRelations.name2Relations();
    StringList results;
    for (auto const & [childrenName, nameReationSet] : name2Relations)
    {
        bool foundHappyName = false;
        for (auto const & [dummy, name2Relation] : name2Relations)
        {
            if (name2Relation.find(childrenName) != name2Relation.cend())
            {
                foundHappyName = true;
                break;
            }
        }

        if (!foundHappyName)
            results.push_front(childrenName);
    }

    return results;
}

StringList ProcessDataFacade::favouriteChildrenNames() const
{
    auto const & name2Relations = m_childrenRelations.name2Relations();
    unsigned maxCount = 1;
    StringList results;
    std::unordered_map<std::string, unsigned> aCount;
    for (auto const & childrenRelation : name2Relations)
    {
        const auto& nameReations = childrenRelation.second;
        for (const auto& nameRelation : nameReations)
        {
            if (bool inserted = !(aCount.emplace(nameRelation, 1)).second)
            {
                auto& count = aCount[nameRelation];
                ++count;

                if (count > maxCount)
                {
                    maxCount = count;
                    results.push_front(nameRelation + ": " + std::to_string(count));
                }
            }
        }
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
        ePrintData
    };


    const std::string_view menu = R"(
Select action:"
    "1 - unloved children"
    "2 - unhappy children"
    "3 - favorite children"
    "4 - print loaded data"
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
            std::cin.clear();
            std::string s;
            std::cin >> s;
            //std::cout << "You entered " << '"' << s << '"' << " please, try again" << std::endl;
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
        case eUserSelect::ePrintData:
            for (auto const & data : DataArray { &m_childrenNames, &m_childrenRelations })
                data->print();
            break;
        case eUserSelect::eExit:
        {
            utils::printArgs("Bye-bye :)");
            readAgain = false;
            break;
        }
        default:
            utils::printArgs("You entered not existed action, please, try again :)");
            break;
        }
    } while (readAgain);
}

DataFile::~DataFile() = default;
IData::~IData() = default;

};//end namespace PaveA
