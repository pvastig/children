#include"data.h"
#include "test.h"
#include "utils.h"

#include <cassert>

using namespace pa;
using namespace utils;

namespace Test
{
constexpr int argc = 3;
static char * argv[] = { "dummy", "../tests/names.dat", "../tests/children_relations.dat" };
#define PRINT_DURATION_TIME  printArgs("Elapsed time: ", DURATION_TIME, utils::newLine)

void printCmdArgs()
{
    printArgs("Pavela ", 1, " test ", 2, newLine);
}

void readDataNames()
{
    ChildrenNames childrenNames;
    childrenNames.read("../tests/test_reading_names.dat");
}

void readDataRelations()
{
    ChildrenRelations childrenRelations;
    childrenRelations.read("../tests/test_reading_relations.dat");
}

void unlovedChildren()
{
    StringList comparedNames = { "Marina", "Richard5", "Vasya" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    auto const & unlovedChildrenNames = prData.unlovedChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    const auto size1 = std::distance(comparedNames.cbegin(), comparedNames.cend());
    const auto size2 = std::distance(unlovedChildrenNames.cbegin(), unlovedChildrenNames.cend());
    assert(size1 == size2);
    for (auto it1 = comparedNames.cbegin(), it2 = unlovedChildrenNames.cbegin(); it1 != comparedNames.cend(); ++it1, ++it2)
    {
        assert(*it1 == *it2);
    }
}

void unhappyChildren()
{
    StringList comparedNames{ "Vasya" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    const auto unhappyChildrenNames = prData.unhappyChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    const auto size1 = std::distance(comparedNames.cbegin(), comparedNames.cend());
    const auto size2 = std::distance(unhappyChildrenNames.cbegin(), unhappyChildrenNames.cend());
    assert(size1 == size2);

    for (auto it1 = comparedNames.cbegin(), it2 = unhappyChildrenNames.cbegin(); it1 != comparedNames.cend(); ++it1, ++it2)
    {
        assert(*it1 == *it2);
    }
}

void favouriteChildren()
{
    StringList comparedNames{ "Masha: 3", "Oleg: 2" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    const auto favouriteChildrenNames = prData.favouriteChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    utils::printContainerData(favouriteChildrenNames);
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
    readDataNames();
    readDataRelations();
    unlovedChildren();
    unhappyChildren();
    favouriteChildren();
}
};
