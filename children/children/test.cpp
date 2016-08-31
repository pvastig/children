#include "stdafx.h"

#include"data.h"
#include "test.h"

using namespace PavelA;

namespace Test
{
  void testTemplates()
  {
    utils::printArgs("Pavela ", 1, " test ", 2, '\n', "new line", '\n');
  }

  void testReadDataNames()
  {
    Names dataNames;
    dataNames.read("test/test_reading_names.dat");
    dataNames.print();
  }

  void testReadDataRelations()
  {
    ChildrenRelations dataRelations;
    dataRelations.read("test/test_reading_relations.dat");
    dataRelations.print();
  }

  void testUnlovedChildrenList()
  {
    int argc = 3;
    char * argv[3];
    argv[0] = "program name";
    argv[1] = "test/test_unloved_children_names.dat";
    argv[2] = "test/test_unloved_children_relations.dat";
    
    /*ProcessData prData(argc, argv);
    const DataPtrArray & aData = prData.getDataPtrArray();
    for (const auto & data : aData)
      data->printData();
      

    const StringArray aUnlovedChildrenNames(prData.unlovedChildrenNames().begin(), prData.unlovedChildrenNames().end());
    StringArray aCompareNames = {"Marina", "Richard5", "Vasya"};
    assert(aCompareNames.size() == aUnlovedChildrenNames.size());

    for (size_t i = 0; i < aCompareNames.size(); ++i)
    {
      const std::string & strCmpName = aCompareNames[i];
      const std::string & strUnlChildName = aUnlovedChildrenNames[i];

      if (strCmpName.compare(strUnlChildName) != 0)
      {
        std::cout << strCmpName << " != " << strUnlChildName << '\n';
        assert(strCmpName.compare(strUnlChildName) == 0);
      }
    }
    printData(aUnlovedChildrenNames);*/
  }

  void testUnhappyChildrenList()
  {
    /*int argc = 3;
    char * argv[3];
    argv[0] = "program name";
    argv[1] = "test/test_unhappy_children_names.dat";
    argv[2] = "test/test_unhappy_children_relations.dat";
    
    ProcessData prData(argc, argv);
    const DataPtrArray & aData = prData.getDataPtrArray();
    for (auto data : aData)
      data->printData();

    const StringArray aUnhappyChildrenNames = prData.unhappyChildrenNames();
    StringArray aCompareNames = {"Vasya"};
    assert(aCompareNames.size() == aUnhappyChildrenNames.size());

    for (size_t i = 0; i < aCompareNames.size(); ++i)
    {
      const std::string & strCmpName = aCompareNames[i];
      const std::string & strUnhappyChildName = aUnhappyChildrenNames[i];

      if (strCmpName.compare(strUnhappyChildName) != 0)
      {
        std::cout << strCmpName << " != " << strUnhappyChildName << '\n';
        assert(strCmpName.compare(strUnhappyChildName) == 0);
      }
    }
    printData(aUnhappyChildrenNames);*/
  }

  void testFavouriteChildrenList()
  {
    int argc = 3;
    char * argv[3];
    argv[0] = "program name";
    argv[1] = "test/test_favourite_children_names.dat";
    argv[2] = "test/test_favourite_children_relations.dat";

    /*ProcessData prData(argc, argv);
    const DataPtrArray & aData = prData.getDataPtrArray();
    for (const auto & data : aData)
      data->printData();*/

    /*StringArray favouriteChildrenNames = prData.favouriteChildrenNames();
    StringArray aCompareNames = {"Oleg", "Masha"};
    assert(aCompareNames.size() == favouriteChildrenNames.size());

    for (size_t i = 0; i < aCompareNames.size(); ++i)
    {
      const std::string & strCmpName = aCompareNames[i];
      const std::string & strFavouriteChildName = favouriteChildrenNames[i];

      if (strCmpName.compare(strFavouriteChildName) != 0)
      {
        std::cout << strCmpName << " != " << strFavouriteChildName << '\n';
        assert(strCmpName.compare(strFavouriteChildName) == 0);
      }
    }
    printData(favouriteChildrenNames);*/
  }

  void testAll()
  {
    Test::testReadDataNames();
    Test::testReadDataRelations();
    Test::testUnlovedChildrenList();
    Test::testUnhappyChildrenList();
    Test::testFavouriteChildrenList();
  }

};