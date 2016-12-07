#include "stdafx.h"

#include"data.h"
#include "test.h"

using namespace PavelA;

namespace Test
{
void printArgs()
{
  utils::printArgs("Pavela ", 1, " test ", 2, '\n', "new line");
}

void readDataNames()
{
  Names dataNames;
  dataNames.read("test/test_reading_names.dat");
  dataNames.print();
}

void readDataRelations()
{
  ChildrenRelations dataRelations;
  dataRelations.read("test/test_reading_relations.dat");
  dataRelations.print();
}

void unlovedChildrenList()
{
  int argc = 3;
  char* argv[3] = { "dummy",
                     "test/test_unloved_children_names.dat",
                     "test/test_unloved_children_relations.dat" };

  ProcessData prData(argc, argv);
  const auto unlovedChildrenNames = prData.unlovedChildrenNames();
  StringList comparedNames = { "Vasya", "Richard5", "Marina" };
  auto size1 = std::distance(comparedNames.cbegin(), comparedNames.cend());
  auto size2 = std::distance(unlovedChildrenNames.cbegin(), unlovedChildrenNames.cend());
  assert(size1 == size2);
  for (auto it1 = comparedNames.cbegin(), it2 = unlovedChildrenNames.cbegin(); it1 != comparedNames.end(); ++it1, ++it2)
  {
    utils::printArgs(NEWLINE, *it1, "--->", *it2);
    assert(*it1 == *it2);
  }
}

void unhappyChildrenList()
{
  int argc = 3;
  char* argv[3] = { "dummy",
                     "test/test_unhappy_children_names.dat",
                     "test/test_unhappy_children_relations.dat" };

  ProcessData prData(argc, argv);
  const auto unhappyChildrenNames = prData.unhappyChildrenNames();
  std::cout << std::endl;
  utils::printData(unhappyChildrenNames);
  StringList comparedNames { "Vasya","Masha" };
  auto size1 = std::distance(comparedNames.cbegin(), comparedNames.cend());
  auto size2 = std::distance(unhappyChildrenNames.cbegin(), unhappyChildrenNames.cend());
  assert(size1 == size2);

  for (auto it1 = comparedNames.cbegin(), it2 = unhappyChildrenNames.cbegin(); it1 != comparedNames.end(); ++it1, ++it2)
  {
    utils::printArgs(NEWLINE, *it1, "--->", *it2);
    assert(*it1 == *it2);
  }
}

void favouriteChildrenList()
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
  Test::readDataNames();
  Test::readDataRelations();
  Test::unlovedChildrenList();
  Test::unhappyChildrenList();
  Test::favouriteChildrenList();
}

};