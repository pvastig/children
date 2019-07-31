#include"../src/data.h"
#include "test.h"
#include "../src/utils.h"

using namespace pa;
using namespace utils;

namespace Test
{
//TODO: make 2 files for testing!
constexpr int argc = 3;
static char const * argv[] = { "dummy", "../names.dat", "../children_relations.dat" };
#define PRINT_DURATION_TIME  printArgs("elapsed time: ", DURATION_TIME, utils::newLine)

//TODO: improve for different types, may be compare different types?
template<class T1, class T2>
void compareContainers(T1 referenceContainer, T2 comparedContainer, int lineError)
{
    auto const size1 = std::distance(referenceContainer.cbegin(), comparedContainer.cend());
    auto const size2 = std::distance(comparedContainer.cbegin(), comparedContainer.cend());
    if (size1 != size2)
        throw std::length_error("Line: "             + std::to_string(lineError) +
                                ". Reference size: " + std::to_string(size1)     +
                                ". Compared size: "  + std::to_string(size2));
    for (auto it1 = referenceContainer.cbegin(), it2 = comparedContainer.cbegin(); it1 != referenceContainer.cend(); ++it1, ++it2)
    {
        if (*it1 != *it2)
            throw std::logic_error("Line: "              + std::to_string(lineError) +
                                    ". Reference name: " + *it1                      +
                                    ". Compared name: "  + *it2);
    }
}

void readDataNames()
{
    printArgs(__func__);
    ChildrenNames names;

    START_TIME;
    names.read("../test_reading_names.dat");
    STOP_TIME;
    PRINT_DURATION_TIME;

    auto const & childrenNames = names.names();
    StringList readNames(childrenNames.cbegin(), childrenNames.cend());
    StringList const referencedNames = { "Vasya", "Mash", "123Geor", "Pertya", "Richard5", "Katya", "1" };
    compareContainers(referencedNames, readNames, __LINE__);
}

void readDataRelations()
{
    printArgs(__func__);
    ChildrenRelations childrenRelations;

    START_TIME;
    childrenRelations.read("../test_reading_relations.dat");
    STOP_TIME;
    PRINT_DURATION_TIME;

    auto const & name2RelatedNames = childrenRelations.name2RelatedNames();
    StringUnordMap const referenceNames = {
        {"Petya", {"Vasya" }},
        {"Vasya", {"Masha", "Katya"}},
        {"Katya", {"123Gerg"}}
    };
    StringList readNames;
    StringUnordSet relatedNames;
    for (auto const & [name, relatedName] : name2RelatedNames)
    {
        readNames.push_front(name);
        relatedNames.insert(relatedName.cbegin(), relatedName.cend());
    }

    StringList refNames;
    StringUnordSet refRelatedNames;
    for (auto const & [name, relatedName] : referenceNames)
    {
        refNames.push_front(name);
        refRelatedNames.insert(relatedName.cbegin(), relatedName.cend());
    }

    compareContainers(readNames, refNames, __LINE__);
    compareContainers(relatedNames, refRelatedNames, __LINE__);
}

void unlovedChildren()
{
    printArgs(__func__);
    StringList const referenceNames = { "Marina", "Richard5", "Vasya" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    auto const unlovedChildrenNames = prData.unlovedChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    compareContainers(referenceNames, unlovedChildrenNames, __LINE__);
}

void unhappyChildren()
{
    printArgs(__func__);
    StringList const referenceNames{ "Vasya" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    auto const unhappyChildrenNames = prData.unhappyChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    compareContainers(referenceNames, unhappyChildrenNames, __LINE__);
}

void favouriteChildren()
{
    printArgs(__func__);
    StringList const referenceNames{ "Masha: 3", "Oleg: 2" };
    ProcessDataFacade prData(argc, argv);

    START_TIME;
    auto const favouriteChildrenNames = prData.favouriteChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME;

    compareContainers(referenceNames, favouriteChildrenNames, __LINE__);
}

//TODO: improve test
void testLog()
{
    LOG.setFileName("testLog.log");
    LOG << "this is big test";
}

void all()
{
    readDataNames();
    readDataRelations();
    unlovedChildren();
    unhappyChildren();
    favouriteChildren();
}
}
