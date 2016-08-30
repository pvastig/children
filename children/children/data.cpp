#include "stdafx.h"

#include "data.h"

namespace PavelA
{
  #define ENDLINE '\n'
  const std::string msgFinishedReading("Reading has been finished");

  void DataNames::read(const std::string& fileName)
  {
    std::ifstream ifs(fileName.c_str(), std::ios_base::in);
    if (!ifs.is_open())
      throw std::invalid_argument("Error getting file: " + fileName);

    m_fileName = fileName;
    std::cout << "\nReading data from " << m_fileName << std::endl;
    std::string curLine;
    while (std::getline(ifs, curLine))
    {
      ++m_countLines;
      //skip empty strings
      if (curLine.empty())
      {
        std::cout << "line:" << m_countLines << " is empty" << ENDLINE;
        continue;
      }

      if (bool inserted = !m_names.insert(curLine).second)
        std::cout << "line:" << m_countLines << " duplicated data: " << curLine << ENDLINE;
    }
    if (ifs.is_open())
      ifs.close();

    std::cout << msgFinishedReading << std::endl;
  }

  void DataNames::print()
  {
    std::cout << "\n**************** Printed data from file " << m_fileName << std::endl;
    printData(m_names);
  }

  void DataRelationsName::read(const std::string & fileNameIn)
  {
    std::ifstream ifs(fileNameIn.c_str(), std::ios_base::in);
    if (!ifs.is_open())
      throw std::invalid_argument("Error getting file:" + fileNameIn);

    m_fileName = fileNameIn;
    std::cout << "\nReading data from " << m_fileName << std::endl;
    std::string curLine;
    while (std::getline(ifs, curLine))
    {
      ++m_countLines;
      //skip empty strings
      if (curLine.empty())
      {
        std::cout << "line:" << m_countLines << " is empty" << '\n';
        continue;
      }

      std::istringstream inStream(curLine);
      std::string word1, word2;

      inStream >> word1 >> word2;
      if (word1.empty() || word2.empty())
      {
        std::cout << "line:" << m_countLines << " has invalid data: " << "\"" << word1 << "\" " << " \"" << word2 << "\"" << '\n';
        continue;
      }

      if (word1 == word2)
        continue;

      m_namesRelations[word1].insert(word2);
    }

    if (ifs.is_open())
      ifs.close();

    std::cout << "Reading has been finished\n";
  }

  void DataRelationsName::print()
  {
    std::cout << "\n**************** Printed data from file " << m_fileName << std::endl;
    for (const auto& nameRelation : m_namesRelations)
    {
      const auto & aStrings = nameRelation.second;
      std::cout << nameRelation.first << '\t' << *aStrings.begin() << '\n';
      for (auto it = ++aStrings.begin(); it != aStrings.end(); ++it)
        std::cout << '\t' << *it << '\n';
    }
  }

  ProcessData::ProcessData(const int argc, char ** argv) : m_argc(argc), m_argv(argv)
  {
    if (argc != 3)
      throw std::invalid_argument("You should enter input files");

    m_dataNames.read(argv[1]);
    m_dataRelationName.read(argv[2]);

    /* for (const auto & data : m_pDataArray)
     {
       assert(data.get());
       if (!data.get())
         throw std::runtime_error("Invalid pointer");

       if (DataNames * pDataNames = dynamic_cast<DataNames*>(data.get()))
         m_names = pDataNames->getData();

       if (DataNamesRelations * pDataRelations = dynamic_cast<DataNamesRelations*>(data.get()))
         m_namesRelations = pDataRelations->getData();
     }*/
  }

  StringList ProcessData::unlovedChildrenNames() const
  {
    const auto& names = m_dataNames.getData();
    const auto& namesRelations = m_dataRelationName.getData();
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
    const auto& names = m_dataNames.getData();
    const auto& namesRelations = m_dataRelationName.getData();

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
    const auto& names = m_dataNames.getData();
    const auto& namesRelations = m_dataRelationName.getData();
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
      std::cout << menu << std::endl;
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
          PavelA::printData(unlovedChildrenNames());
          break;
        }
        case eUserSelect::eUnhappyChildrenNames:
        {
          PavelA::printData(unhappyChildrenNames());
          break;
        }
        case eUserSelect::eFavouriteChildrenNames:
        {
          PavelA::printData(favouriteChildrenNames());
          break;
        }
        case eUserSelect::ePrintData:
        {
          //it needs to improve
          /*          const PavelA::DataPtrArray & aData = prData.getDataPtrArray();
          for (const auto & data : aData)
          data->printData();*/
          break;
        }
        case eUserSelect::eExit:
        {
          std::cout << "Bye-bye :)" << std::endl;
          readAgain = false;
          break;
        }
        default:
        std::cout << "You entered not existed action, please, try again" << std::endl;
        break;
      }

    } while (readAgain);
  }

};//end namespace PaveA
