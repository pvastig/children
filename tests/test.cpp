#include "test.h"

#include "../src/data.h"
#include "../src/utils.h"

#include <thread>

using namespace pa;
using namespace utils;

namespace Test
{
#define PRINT_DURATION_TIME(str) printArgs(str, DURATION_TIME, newLine)
#define FILE_LINE __LINE__
#define PRINT_FUNC_NAME printArgs(newLine, __func__, ":", newLine)

static char const * argv[] = { "dummy", "../names.dat", "../children_relations.dat" };
constexpr int argc = 3;
constexpr char const * strDataReading = "Data reading: ";
constexpr char const * strFuncExec    = "Execution: ";

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
                                   ". Reference name: " + *it1                       +
                                   ". Compared name: "  + *it2);
    }
}

void readDataNames()
{
    PRINT_FUNC_NAME;
    ChildrenNames names;

    START_TIME;
    names.read("../test_reading_names.dat");
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

    auto const & childrenNames = names.names();
    StringList readNames(childrenNames.cbegin(), childrenNames.cend());
    StringList const referencedNames = { "Vasya", "Mash", "123Geor", "Pertya", "Richard5", "Katya", "1" };
    compareContainers(referencedNames, readNames, FILE_LINE);
}

void readDataRelations()
{
    PRINT_FUNC_NAME;
    ChildrenRelations childrenRelations;

    START_TIME;
    childrenRelations.read("../test_reading_relations.dat");
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

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

    compareContainers(readNames, refNames, FILE_LINE);
    compareContainers(relatedNames, refRelatedNames, FILE_LINE);
}

void unlovedChildren()
{
    PRINT_FUNC_NAME;
    StringList const referenceNames = { "Marina", "Richard5", "Vasya" };
    START_TIME;
    ProcessDataFacade prData(argc, argv);
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

    START_TIME;
    auto const unlovedChildrenNames = prData.unlovedChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME(strFuncExec);

    compareContainers(referenceNames, unlovedChildrenNames, FILE_LINE);
}

void unhappyChildren()
{
    PRINT_FUNC_NAME;
    StringList const referenceNames{ "Vasya" };

    START_TIME;
    ProcessDataFacade prData(argc, argv);
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

    START_TIME;
    auto const unhappyChildrenNames = prData.unhappyChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME(strFuncExec);

    compareContainers(referenceNames, unhappyChildrenNames, FILE_LINE);
}

void favouriteChildren()
{
    PRINT_FUNC_NAME;
    StringList const referenceNames{ "Masha: 3", "Oleg: 2" };

    START_TIME;
    ProcessDataFacade prData(argc, argv);
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

    START_TIME;
    auto const favouriteChildrenNames = prData.favouriteChildrenNames();
    STOP_TIME;
    PRINT_DURATION_TIME(strFuncExec);

    compareContainers(referenceNames, favouriteChildrenNames, FILE_LINE);
}

//TODO: make big files for reading data
void testLog()
{
    LOG.setFileName("testLog");
    LOG << "this is big test";
}

void testConcurrencyReading()
{
    PRINT_FUNC_NAME;

    {
        Timer totalTime;
        totalTime.start();
        {
            START_TIME;
            ChildrenNames().read(argv[1]);
            STOP_TIME;
            PRINT_DURATION_TIME("First reading: ");
        }

        {
            START_TIME;
            ChildrenRelations().read(argv[2]);
            STOP_TIME;
            PRINT_DURATION_TIME("Second reading: ");
        }
        totalTime.stop();
        printArgs("Total: ", totalTime.duration(), newLine);
    }
    {
        Timer totalTime;
        totalTime.start();
        {
            START_TIME;
            std::thread th1([]() { ChildrenNames().read(argv[1]); });
            th1.join();
            STOP_TIME;
            PRINT_DURATION_TIME("First thread: ");
        }

        {
            START_TIME;
            std::thread th2([]() { ChildrenRelations().read(argv[2]);});
            th2.join();
            STOP_TIME;
            PRINT_DURATION_TIME("Second thread: ");
        }
        totalTime.stop();
        printArgs("Total: ", totalTime.duration(), newLine);
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
}
