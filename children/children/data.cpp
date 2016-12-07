#include "stdafx.h"

#include "data.h"

namespace PavelA
{
const std::string msgFinishedReading("Reading has been finished");
const std::string msgPintedDataFromFile("**************** Printed data from file ");

void Names::read(const std::string& fileName)
{
  std::ifstream ifs(fileName.c_str(), std::ios_base::in);
  if (!ifs.is_open())
    throw std::invalid_argument("Error getting file: " + fileName);

  m_fileName = fileName;
  utils::printArgs(NEWLINE, "Reading data from ", m_fileName);
  std::string curLine;
  while (std::getline(ifs, curLine))
  {
    ++m_countLines;
    //skip empty strings
    if (curLine.empty())
    {
      utils::printArgs(NEWLINE, "line: ", m_countLines, " is empty");
      continue;
    }

    if (bool inserted = !m_names.insert(curLine).second)
      utils::printArgs(NEWLINE, "line: ", m_countLines, " duplicated data : ", curLine);
  }

  utils::printArgs(NEWLINE, msgFinishedReading);
}

void Names::print() const
{
  utils::printArgs(NEWLINE, msgPintedDataFromFile, m_fileName);
  utils::printData(m_names);
}

void ChildrenRelations::read(const std::string& fileName)
{
  std::ifstream ifs(fileName.c_str(), std::ios_base::in);
  if (!ifs.is_open())
    throw std::invalid_argument("Error getting file:" + fileName);

  m_fileName = fileName;
  utils::printArgs(NEWLINE, "Reading data from ", m_fileName);

  std::string curLine;
  while (std::getline(ifs, curLine))
  {
    ++m_countLines;
    //skip empty strings
    if (curLine.empty())
    {
      utils::printArgs(NEWLINE, "line: ", m_countLines, "is empty");
      continue;
    }

    std::istringstream inStream(curLine);
    std::string word1, word2;
    inStream >> word1 >> word2;
    if (word1.empty() || word2.empty())
    {
      utils::printArgs(NEWLINE, "line: ", m_countLines, " has invalid data: \"", word1, "\"\"", word2, "\"");
      continue;
    }

    if (word1 == word2)
      continue;

    m_namesRelations[word1].insert(word2);
  }

  utils::printArgs(NEWLINE, msgFinishedReading);
}

void ChildrenRelations::print() const
{
  utils::printArgs(NEWLINE, msgPintedDataFromFile, m_fileName);
  for (const auto& nameRelation : m_namesRelations)
  {
    const auto & aStrings = nameRelation.second;
    if (aStrings.empty())
      continue;

    const auto& firstName = nameRelation.first;
    auto firstRelationName = aStrings.begin();

    utils::printArgs(NEWLINE, firstName, '\t', *firstRelationName, NEWLINE);
    std::for_each(std::next(aStrings.begin(), 1), aStrings.end(),
                  [](const auto& relationName)
    {
      utils::printArgs('\t', relationName, NEWLINE);
    }
    );
  }
}

ProcessData::ProcessData(const int argc, char ** const argv)
  : m_argc(argc), m_argv(argv)
{
  if (argc != 3)
    throw std::invalid_argument("You should enter input files");

  m_names.read(argv[1]);
  m_childrenRelations.read(argv[2]);
}

StringList ProcessData::unlovedChildrenNames() const
{
  const StringSet names(m_names.getData().cbegin(), m_names.getData().cend());
  const auto& childrenRelations = m_childrenRelations.getData();

  StringSet namesRelations;
  for (const auto& nameRelation : childrenRelations)
  {
    const auto& nameReationSet = nameRelation.second;
    namesRelations.insert(nameReationSet.begin(), nameReationSet.end());
  }

  auto intersectSets = [](const auto& strSet1, const auto& strSet2)
  {
    auto begin1 = strSet1.cbegin();
    auto end1 = strSet1.cend();
    auto begin2 = strSet2.cbegin();
    auto end2 = strSet2.cend();
    if (strSet1.size() < strSet2.size())
    {
      std::swap(begin1, begin2);
      std::swap(end1, end2);
    }

    StringList results;
    std::set_difference(begin1, end1, begin2, end2, std::front_inserter(results));

    return results;
  };

  return intersectSets(names, namesRelations);
}

StringList ProcessData::unhappyChildrenNames() const
{
  const auto& names = m_names.getData();
  const auto& childrenRelations = m_childrenRelations.getData();
  StringList unhappyChildrenNames;
  for (const auto& name : names)
  {
    bool foundHappyName = false;
    for (const auto& relationName : childrenRelations)
    {
      const auto foundName = relationName.second.find(name);
      if (foundName != relationName.second.end())
      {
        foundHappyName = true;
        break;
      }
    }
    if (!foundHappyName)
      unhappyChildrenNames.push_front(name);
  }

  return unhappyChildrenNames;
}

StringList ProcessData::favouriteChildrenNames() const
{
  const auto& names = m_names.getData();
  const auto& namesRelations = m_childrenRelations.getData();
  std::multiset<std::string> namesMultiSet;
  //collect all names from liked children
  for (const auto& nameRelation : namesRelations)
    namesMultiSet.insert(nameRelation.second.begin(), nameRelation.second.end());

  //count names
  std::multimap<size_t, std::string> countNames; //need to sort names using count
  for (const auto& name : names)
  {
    const auto count = namesMultiSet.count(name);
    if (0 == count)
      continue;

    countNames.emplace(count, name);
  }

  utils::printDataMap(countNames);
  const auto maxCount = countNames.crbegin()->first;
  StringList aFavouriteChildrenNames{ countNames.crbegin()->second };
  for (auto it = ++countNames.crbegin(); it != countNames.crend(); ++it)
  {
    if (it->first == maxCount)
      aFavouriteChildrenNames.push_front(it->second);
    else
      break;
  }

  return aFavouriteChildrenNames;
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
