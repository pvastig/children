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
        utils::printArgs(NEWLINE, "line: ", m_countLines, "is empty");
        continue;
      }

      if (bool inserted = !m_names.insert(curLine).second)
        utils::printArgs(NEWLINE, "line: ", m_countLines, " duplicated data : ", curLine);
    }
    if (ifs.is_open())
      ifs.close();

    utils::printArgs(NEWLINE, msgFinishedReading);
  }

  void Names::print()
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

    if (ifs.is_open())
      ifs.close();

    utils::printArgs(NEWLINE, msgFinishedReading);
  }

  void ChildrenRelations::print()
  {
    utils::printArgs(NEWLINE, msgPintedDataFromFile, m_fileName);
    for (const auto& nameRelation : m_namesRelations)
    {
      const auto & aStrings = nameRelation.second;
      if (aStrings.empty())
        continue;
      utils::printArgs(NEWLINE, nameRelation.first, '\t', *aStrings.begin());
      for (auto it = ++aStrings.begin(); it != aStrings.end(); ++it)
        utils::printArgs('\t', *it, '\n');
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
    const auto& names = m_names.getData();
    const auto& namesRelations = m_childrenRelations.getData();
    StringSet namesRelSet;
    for (const auto & nameRelation : namesRelations)
    {
      const StringUnordSet & nameReationSet = nameRelation.second;
      namesRelSet.insert(nameReationSet.begin(), nameReationSet.end());
    }
    StringList unlovedChildrenNames;
    std::set_difference(names.begin(), names.end(), namesRelSet.begin(), namesRelSet.end(), std::front_inserter(unlovedChildrenNames));

    return unlovedChildrenNames;
  }

  StringList ProcessData::unhappyChildrenNames() const
  {
    const auto& names = m_names.getData();
    const auto& namesRelations = m_childrenRelations.getData();

    StringList unhappyChildrenNames;
    StringUnordSet happyNames;
    for (const auto & name : names)
    {
      const auto foundName = namesRelations.find(name);
      if (foundName == namesRelations.end())
        continue;

      const auto & foundNamesSet = foundName->second;
      for (const auto & foundName : foundNamesSet)
      {
        const auto found = namesRelations.find(foundName);
        if (found == namesRelations.end())
          continue;

        const auto & checkedNames = found->second;
        if ((checkedNames.find(name) == checkedNames.end()) && (happyNames.find(name) == happyNames.end()))
          unhappyChildrenNames.push_front(name);
        else
          happyNames.insert(name);
      }
    }

    return unhappyChildrenNames;
  }

  StringList ProcessData::favouriteChildrenNames() const
  {
    const auto& names = m_names.getData();
    const auto& namesRelations = m_childrenRelations.getData();
    std::multiset<std::string> namesMultiSet;
    //collect all names from liked children
    for (const auto & nameRelation : namesRelations)
      namesMultiSet.insert(nameRelation.second.begin(), nameRelation.second.end());

    //count names
    std::multiset<std::pair<size_t, std::string>> countNamesMultiSet;
    for (const auto & name : names)
    {
      const size_t countNames = namesMultiSet.count(name);
      if (0 == countNames)
        continue;

      countNamesMultiSet.emplace(countNames, name);
    }

    StringList aFavouriteChildrenNames;
    const size_t maxCount = countNamesMultiSet.rbegin()->first;
    aFavouriteChildrenNames.push_front(countNamesMultiSet.rbegin()->second);
    for (auto it = ++countNamesMultiSet.rbegin(); it != countNamesMultiSet.rend(); ++it)
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
          DataPtrArray aData { &m_names, &m_childrenRelations };
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
