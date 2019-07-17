#include"../src/data.h"
#include "test.h"
#include "../src/utils.h"

#undef NDEBUG //to use for release configuration
#include <cassert>

using namespace pa;
using namespace utils;

namespace Test
{
constexpr int argc = 3;
static char * argv[] = { "dummy", "../names.dat", "../children_relations.dat" };
#define PRINT_DURATION_TIME  printArgs("elapsed time: ", DURATION_TIME, utils::newLine)

//TODO: improve for different types, may be compare different types?
template<class T1, class T2>
bool cmpContainers(T1 container1, T2 container2)
{
    auto const size1 = std::distance(container1.cbegin(), container2.cend());
    auto const size2 = std::distance(container2.cbegin(), container2.cend());
    if (size1 != size2)
        return false;
    for (auto it1 = container1.cbegin(), it2 = container2.cbegin(); it1 != container1.cend(); ++it1, ++it2)
    {
        if (*it1 != *it2)
        {
            printArgs(*it1, ' ', *it2);
            return false;
        }
    }
    return true;
}

void readDataNames()
{
    printArgs("readDataNames: ");
    ChildrenNames childrenNames;

    START_TIME;
    childrenNames.read("../test_reading_names.dat");
    STOP_TIME;
    PRINT_DURATION_TIME;

    auto const & readNames = childrenNames.childrenNames();
    StringList res(readNames.cbegin(), readNames.cend());
    StringList const comparedNames = { "Vasya", "Mash", "123Geor", "Pertya", "Richard5", "Katya", "1" };
    assert(cmpContainers(comparedNames, res));
}

void readDataRelations()
{
    printArgs("readDataNames: ");
    ChildrenRelations childrenRelations;

    START_TIME;
    childrenRelations.read("../test_reading_relations.dat");
    STOP_TIME;
    PRINT_DURATION_TIME;

    auto const & readNames = childrenRelations.name2RelatedNames();
    StringUnordMap const comparedNames = {
        {"Petya", {"Vasya" }},
        {"Vasya", {"Masha", "Katya"}},
        {"Katya", {"123Gerg"}}
    };
    StringList res1;
    StringUnordSet res2;
    for (auto const & [name, relatedName] : readNames)
    {
        res1.push_front(name);
        res2.insert(relatedName.cbegin(), relatedName.cend());
    }

    StringList res3;
    StringUnordSet res4;
    for (auto const & [name, relatedName] : comparedNames)
    {
        res3.push_front(name);
        res4.insert(relatedName.cbegin(), relatedName.cend());
    }

    assert(cmpContainers(res1, res3));
    assert(cmpContainers(res2, res4));
}

void unlovedChildren()
{
    printArgs("unlovedChildren: ");
    StringList const comparedNames = { "Marina", "Richard5", "Vasya" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    auto const unlovedChildrenNames = prData.unlovedChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    assert(cmpContainers(comparedNames, unlovedChildrenNames));
}

void unhappyChildren()
{
    printArgs("unhappyChildren: ");
    StringList const comparedNames{ "Vasya" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    auto const unhappyChildrenNames = prData.unhappyChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    assert(cmpContainers(comparedNames, unhappyChildrenNames));
}

void favouriteChildren()
{
    printArgs("favouriteChildren: ");
    StringList const comparedNames{ "Masha: 3", "Oleg: 2" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    auto const favouriteChildrenNames = prData.favouriteChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    assert(cmpContainers(comparedNames, favouriteChildrenNames));
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
