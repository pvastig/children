#include "data.h"

#include "utils.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace pa
{
const std::string msgFinishedReading("Reading has been finished");
const std::string msgPintedFileData("**************** Printed data from file ");
constexpr char const * const NEWLINE = "\n";

void Names::read(const std::string& fileName)
{
  std::ifstream ifs(fileName.c_str(), std::ios_base::in);
  if (!ifs.is_open())
    throw std::invalid_argument("Error getting file: " + fileName);

  m_fileName = fileName;
  utils::printArgs(NEWLINE, "Reading data from ", m_fileName);
  std::string line;
  while (std::getline(ifs, line))
  {
    ++m_countLines;
    //skip empty strings
    if (line.empty())
    {
      utils::printArgs(NEWLINE, m_countLines, " is empty");
      continue;
    }

    if (bool inserted = !m_names.insert(line).second)
      utils::printArgs(NEWLINE, m_countLines, " duplicated data : ", line);
  }

  utils::printArgs(NEWLINE, msgFinishedReading);
}

void Names::print() const
{
  utils::printArgs(NEWLINE, msgPintedFileData, m_fileName);
  utils::printData(m_names);
}

void ChildrenRelations::read(const std::string& fileName)
{
  std::ifstream ifs(fileName.c_str(), std::ios_base::in);
  if (!ifs.is_open())
    throw std::invalid_argument("Error getting file: " + fileName);

  m_fileName = fileName;
  utils::printArgs(NEWLINE, "Reading data from ", m_fileName);

  std::string line;
  std::istringstream iss(line);
  while (std::getline(ifs, line))
  {
    ++m_countLines;
    if (line.empty())
    {
      utils::printArgs(NEWLINE,  m_countLines, " is empty");
      continue;
    }

    iss.clear();
    iss.str(line);
    std::string word1, word2;
    if (!(iss >> word1 >> word2))
    {
      utils::printArgs(NEWLINE, m_countLines, " has invalid data: ", line);
      continue;
    }

    if (word1 == word2)
    {
      utils::printArgs(NEWLINE, m_countLines, " has same words: ", line);
      continue;
    }

    m_nameRelations[word1].insert(word2);
  }

  utils::printArgs(NEWLINE, msgFinishedReading);
}

void ChildrenRelations::print() const
{
  utils::printArgs(NEWLINE, msgPintedFileData, m_fileName);
  for (const auto& nameRelation : m_nameRelations)
  {
    const auto& names = nameRelation.second;
    if (names.empty())
      continue;

    const auto& firstName = nameRelation.first;
    const auto& firstRelationName = names.begin();
    utils::printArgs(NEWLINE, firstName, '\t', *firstRelationName, NEWLINE);
    std::for_each(std::next(names.begin()), names.end(),
                  [](const auto& relationName)
                  {
                    utils::printArgs('\t', relationName, NEWLINE);
                  }
    );
  }
}

ProcessData::ProcessData(const int argc, const char ** argv)
  : m_argc(argc), m_argv(argv)
{
  if (argc != 3)
    throw std::invalid_argument("You should enter input files");

  m_names.read(argv[1]);
  m_childrenRelations.read(argv[2]);
}

StringList ProcessData::unlovedChildrenNames() const
{
  const auto& names = m_names.getData();
  const auto& childrenRelations = m_childrenRelations.getData();
  StringList results;
  for (const auto& name : names)
  {
    bool foundHappyName = false;
    for (const auto& childrenRelation : childrenRelations)
    {
      const auto& nameReationSet = childrenRelation.second;
      if (nameReationSet.find(name) != nameReationSet.end())
      {
        foundHappyName = true;
          break;
      }
    }

    if (!foundHappyName)
      results.push_front(name);
  }

  return results;
}

StringList ProcessData::unhappyChildrenNames() const
{
  const auto& names = m_names.getData();
  const auto& childrenRelations = m_childrenRelations.getData();
  StringList results;
  for (const auto& item : childrenRelations)
  {
    const auto& name = item.first;
    bool foundHappyName = false;
    for (const auto& childrenName : childrenRelations)
    {
      const auto& nameReationSet = childrenName.second;
      if (nameReationSet.find(name) != nameReationSet.end())
      {
        foundHappyName = true;
        break;
      }
    }

    if (!foundHappyName)
      results.push_front(name);
  }

  return results;
}

StringList ProcessData::favouriteChildrenNames() const
{
  const auto& names = m_names.getData();
  const auto& childrenRelations = m_childrenRelations.getData();
  unsigned maxCount = 1;
  StringList results;
  std::unordered_map<std::string, unsigned> aCount;
  for (const auto& childrenRelation : childrenRelations)
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

void ProcessData::run()
{
  enum class eUserSelect : char
  {
    eExit = 0,
    eUnlovedChildrenNames,
    eUnhappyChildrenNames,
    eFavouriteChildrenNames,
    ePrintData
  };


  const std::string menu = "\nPlease, select action:\n"
    "1 - unloved children\n"
    "2 - unhappy children\n"
    "3 - favorite children\n"
    "4 - print loaded data\n"
    "------------------------\n"
    "0 - exit\n";

  int num = 0;
  bool readAgain = true;
  do
  {
    utils::printArgs(menu);
    std::cin >> num;
    if (!std::cin)
    {
      std::cin.clear();
      std::string s;
      std::cin >> s;
      std::cout << "You entered " << '"' << s << '"' << " please, try again" << std::endl;;
    }
    switch ((eUserSelect)num)
    {
      case eUserSelect::eUnlovedChildrenNames:
      {
        utils::printData(unlovedChildrenNames());
        break;
      }
      case eUserSelect::eUnhappyChildrenNames:
      {
        utils::printData(unhappyChildrenNames());
        break;
      }
      case eUserSelect::eFavouriteChildrenNames:
      {
        utils::printData(favouriteChildrenNames());
        break;
      }
      case eUserSelect::ePrintData:
      {
        DataPtrArray aData{ &m_names, &m_childrenRelations };
        for (const auto& data : aData)
          data->print();

        break;
      }
      case eUserSelect::eExit:
      {
        utils::printArgs(NEWLINE, "Bye-bye :)");
        readAgain = false;
        break;
      }
      default:
        utils::printArgs(NEWLINE, "You entered not existed action, please, try again");
        break;
    }

  } while (readAgain);
}

};//end namespace PaveA
