#include "stdafx.h"

#include"data.h"
#include "test.h"

using namespace PavelA;

namespace Test
{
int argc = 3;
char* argv[3] = { "dummy", "test/names.dat", "test/children_relations.dat" };

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
  StringList comparedNames = { "Vasya", "Marina", "Richard5"};
  ProcessData prData(argc, argv);

  utils::Time t;
  START;
  const auto unlovedChildrenNames = prData.unlovedChildrenNames();
  END;
  utils::printArgs(NEWLINE, DURATION, NEWLINE);

  const auto size1 = std::distance(comparedNames.cbegin(), comparedNames.cend());
  const auto size2 = std::distance(unlovedChildrenNames.cbegin(), unlovedChildrenNames.cend());
  assert(size1 == size2);
  for (auto it1 = comparedNames.cbegin(), it2 = unlovedChildrenNames.cbegin(); it1 != comparedNames.cend(); ++it1, ++it2)
  {
    assert(*it1 == *it2);
  }
}

void unhappyChildrenList()
{
  StringList comparedNames{ "Vasya" };
  ProcessData prData(argc, argv);

  utils::Time t;
  START;
  const auto unhappyChildrenNames = prData.unhappyChildrenNames();
  END;
  utils::printArgs(NEWLINE, DURATION, NEWLINE);

  const auto size1 = std::distance(comparedNames.cbegin(), comparedNames.cend());
  const auto size2 = std::distance(unhappyChildrenNames.cbegin(), unhappyChildrenNames.cend());
  assert(size1 == size2);

  for (auto it1 = comparedNames.cbegin(), it2 = unhappyChildrenNames.cbegin(); it1 != comparedNames.cend(); ++it1, ++it2)
  {
    assert(*it1 == *it2);
  }
}

void favouriteChildrenList()
{
  StringList comparedNames{ "Masha: 3", "Oleg: 2" };
  ProcessData prData(argc, argv);

  utils::Time t;
  START;
  const auto favouriteChildrenNames = prData.favouriteChildrenNames();
  END;
  utils::printArgs(NEWLINE, DURATION, NEWLINE);
  utils::printData(favouriteChildrenNames);
  const auto size1 = std::distance(comparedNames.cbegin(), comparedNames.cend());
  const auto size2 = std::distance(favouriteChildrenNames.cbegin(), favouriteChildrenNames.cend());
  assert(size1 == size2);

  for (auto it1 = comparedNames.cbegin(), it2 = favouriteChildrenNames.cbegin(); it1 != comparedNames.cend(); ++it1, ++it2)
  {
    assert(*it1 == *it2);
  }
}

void all()
{
  Test::readDataNames();
  Test::readDataRelations();
  Test::unlovedChildrenList();
  Test::unhappyChildrenList();
  Test::favouriteChildrenList();
}
};