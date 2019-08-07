#include "test.h"

#include "../src/data.h"
#include "../src/utils.h"

#include <thread>
#include <set>

using namespace pa;
using namespace utils;

namespace Test
{
#define PRINT_DURATION_TIME(str) print(str, DURATION_TIME, newLine)
#define FILE_LINE __LINE__
#define PRINT_FUNC_NAME print(newLine, __func__, ": ", newLine)

static char const * argv[] = { "dummy", "../names.dat", "../children_relations.dat" };
constexpr int argc = 3;
constexpr char const * strDataReading = "Data reading: ";
constexpr char const * strFuncExec    = "Execution: ";

template<class T1, class T2>
void compareContainers(T1 referenceContainer, T2 comparedContainer, int lineError)
{
    //TODO: if container has size, use size, otherwise use distance
    auto const size1 = std::distance(referenceContainer.cbegin(), referenceContainer.cend());
    auto const size2 = std::distance(comparedContainer.cbegin(), comparedContainer.cend());
    if (size1 != size2)
    {
        printContainer(comparedContainer);
        throw std::length_error("Line: "             + std::to_string(lineError) +
                                ". Reference size: " + std::to_string(size1)     +
                                ". Compared size: "  + std::to_string(size2));
    }
    for (auto it1 = referenceContainer.cbegin(), it2 = comparedContainer.cbegin(); it1 != referenceContainer.cend(); ++it1, ++it2)
    {
        if (*it1 != *it2)
        {
            printContainer(comparedContainer);
            throw std::logic_error("Line: "              + std::to_string(lineError) +
                                   ". Reference name: " + *it1                       +
                                   ". Compared name: "  + *it2);
        }
    }
}

void readDataNames()
{
    PRINT_FUNC_NAME;
    ChildrenNames names;

    START_TIME;
    names.read(argv[1]);
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

    auto const & childrenNames = names.names();
    StringList readNames(childrenNames.cbegin(), childrenNames.cend());
    StringList const referencedNames = { "Oleg", "Vasya","Masha", "Richard5", "123Georg", "Katya", "Petya", "Marina" };
    compareContainers(referencedNames, readNames, FILE_LINE);
}

void readDataRelations()
{
    PRINT_FUNC_NAME;
    ChildrenRelations childrenRelations;

    START_TIME;
    childrenRelations.read(argv[2]);
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

    auto const & name2RelatedNames = childrenRelations.name2RelatedNames();
    StringList readNames;
    std::set<std::string> relatedNames;
    for (auto const & [name, relatedName] : name2RelatedNames)
    {
        readNames.push_front(name);
        relatedNames.insert(relatedName.cbegin(), relatedName.cend());
    }

    StringUnordMap const referenceNames = {
        {"Vasya", {"Masha", "Oleg", "123Georg"}},
        {"Masha", {"Petya", "Oleg", "Katya"}},
        {"Katya", {"Masha"}},
        {"Oleg",  {"Masha"}}
    };
    StringList refNames;
    std::set<std::string> refRelatedNames;
    for (auto const & [name, relatedName] : referenceNames)
    {
        refNames.push_front(name);
        refRelatedNames.insert(relatedName.cbegin(), relatedName.cend());
    }

    compareContainers(refNames, readNames, FILE_LINE);
    compareContainers(refRelatedNames, relatedNames, FILE_LINE);
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

void concurrencyReading()
{
    PRINT_FUNC_NAME;
    std::string_view fileNames = "../names_cuncurrency_test.dat";

    {
        print("Without cuncurrency:", newLine);
        Timer totalTime;
        totalTime.start();
        {
            START_TIME;
            ChildrenNames().read(fileNames);
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
        print("Total: ", totalTime.duration(), newLine);
    }
    {
        print("With cuncurrency:", newLine);
        Timer totalTime;
        totalTime.start();
        {
            START_TIME;
            //TODO: try async mode to take a look at results
            std::thread th1([fileNames]() { ChildrenNames().read(fileNames); });
            th1.join();
            STOP_TIME;
            PRINT_DURATION_TIME("First thread: ");
        }

        {
            START_TIME;
            //TODO: make large file for children relation
            std::thread th2([]() { ChildrenRelations().read(argv[2]);});
            th2.join();
            STOP_TIME;
            PRINT_DURATION_TIME("Second thread: ");
        }
        totalTime.stop();
        print("Total: ", totalTime.duration(), newLine);
    }
}

void all()
{
    readDataNames();
    readDataRelations();
    unlovedChildren();
    unhappyChildren();
    favouriteChildren();
    concurrencyReading();
}
}
