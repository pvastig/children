#include "test.h"

#include "../src/data.h"
#include "../src/utils.h"

#include <future>
#include <set>
#include <type_traits>

using namespace pa;
using namespace utils;

namespace Test {

template<class... Args>
void print(Args... args)
{
    utils::print(std::cout, args...);
}

#define PRINT_DURATION_TIME(str) print(str, DURATION_TIME, newLine)
#define FILE_LINE __LINE__
#define PRINT_FUNC_NAME print(newLine, __PRETTY_FUNCTION__, ": ", newLine)

static char const* argv[] = {"dummy", "../names.dat", "../children_relations.dat"};
//constexpr int argc = 3;
constexpr char const* strDataReading = "Data reading time: ";
constexpr char const* strFuncExec = "Execution time: ";
using StringSet = std::set<std::string>;

pa::StringUnordSet const names
    = {"Oleg", "Vasya", "Masha", "Richard5", "123Georg", "Katya", "Petya", "Marina"};

pa::StringUnordMap const name2RelatedNames{{"Oleg", {"Masha"}},
                                           {"Katya", {"Masha"}},
                                           {"Masha", {"Petya", "Oleg", "Katya"}},
                                           {"Vasya", {"Masha", "Oleg", "123Georg"}}};

template<typename T, typename = void>
struct HasSize : std::false_type
{};

template<typename T>
struct HasSize<T, std::enable_if_t<std::is_same<decltype(std::declval<T>().size()), size_t>::value>>
    : std::true_type
{};

template<typename T>
constexpr bool HasSizeV = HasSize<T>::value;

template<class ReferenceContainer, class ComparedContainer>
void compareContainers(ReferenceContainer refContainer,
                       ComparedContainer cmpContainer,
                       int lineError)
{
    int size1 = 0;
    int size2 = 0;
    if constexpr (HasSizeV<ReferenceContainer> && HasSizeV<ComparedContainer>) {
        size1 = refContainer.size();
        size2 = cmpContainer.size();
    } else {
        size1 = std::distance(refContainer.cbegin(), refContainer.cend());
        size2 = std::distance(cmpContainer.cbegin(), cmpContainer.cend());
    }
    if (size1 != size2) {
        //        print(cmpContainer);
        throw std::length_error("Line: " + std::to_string(lineError)
                                + ". Reference size: " + std::to_string(size1)
                                + ". Compared size: " + std::to_string(size2));
    }
    for (auto it1 = refContainer.cbegin(), it2 = cmpContainer.cbegin(); it1 != refContainer.cend();
         ++it1, ++it2) {
        if (*it1 != *it2) {
            //            print(cmpContainer);
            throw std::logic_error("Line: " + std::to_string(lineError)
                                   + ". Reference name: " + *it1 + ". Compared name: " + *it2);
        }
    }
}

void checkValue(bool val, int lineError)
{
    if (!val)
        throw std::logic_error("Line: " + std::to_string(lineError) + ". is false!");
}

void readChildrenNames()
{
    PRINT_FUNC_NAME;
    ChildrenNamesFile childrenNames(argv[1]);

    START_TIME;
    bool res = childrenNames.read();
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

    checkValue(res, FILE_LINE);

    auto const& names = childrenNames.childrenNames();
    StringList const readNames(names.cbegin(), names.cend());
    StringList const referencedNames
        = {"Oleg", "Vasya", "Masha", "Richard5", "123Georg", "Katya", "Petya", "Marina"};
    compareContainers(referencedNames, readNames, FILE_LINE);
}

void readChildrenRelations()
{
    PRINT_FUNC_NAME;
    ChildrenRelationsFile childrenRelations(argv[2]);

    START_TIME;
    auto res = childrenRelations.read();
    STOP_TIME;
    PRINT_DURATION_TIME(strDataReading);

    checkValue(res, FILE_LINE);

    auto const& name2RelatedNames = childrenRelations.name2RelatedNames();
    StringList readNames;
    StringSet relatedNames;
    for (auto const& [name, relatedName] : name2RelatedNames) {
        readNames.push_front(name);
        relatedNames.insert(relatedName.cbegin(), relatedName.cend());
    }
    StringList const refNames{"Vasya", "Masha", "Katya", "Oleg"};
    StringSet const refRelatedNames{"Masha", "Oleg", "123Georg", "Petya", "Katya"};
    compareContainers(refNames, readNames, FILE_LINE);
    compareContainers(refRelatedNames, relatedNames, FILE_LINE);
}

void unlovedChildren()
{
    PRINT_FUNC_NAME;
    START_TIME;
    auto const unlovedNames = unlovedChildrenNames(names, name2RelatedNames);
    STOP_TIME;
    PRINT_DURATION_TIME(strFuncExec);
    StringList const referenceNames = {"Vasya", "Richard5", "Marina"};
    compareContainers(referenceNames, unlovedNames, FILE_LINE);
}

void unhappyChildren()
{
    PRINT_FUNC_NAME;
    START_TIME;
    auto const unhappyNames = unhappyChildrenNames(name2RelatedNames);
    STOP_TIME;
    PRINT_DURATION_TIME(strFuncExec);
    StringList const referenceNames{"Vasya"};
    compareContainers(referenceNames, unhappyNames, FILE_LINE);
}

void favoriteChildren()
{
    PRINT_FUNC_NAME;
    START_TIME;
    auto const favoriteNames = favoriteChildrenNames(name2RelatedNames);
    STOP_TIME;
    PRINT_DURATION_TIME(strFuncExec);
    StringList const referenceNames{"Oleg: 2", "Masha: 3"};
    compareContainers(referenceNames, favoriteNames, FILE_LINE);
}

/*void concurrencyReading()
{
    using namespace pa;
    using namespace utils;

    // TODO: make big files to check times
    PRINT_FUNC_NAME;
    std::string_view fileNames = "../names_concurrency_test.dat";
    {
        print("Without concurrency:", newLine);
        Timer totalTime;
        totalTime.start();
        {
            START_TIME;
            ChildrenNamesFile().read(fileNames);
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
        print("With concurrency:", newLine);
        Timer totalTime;
        std::future<void> fut1;
        totalTime.start();

        START_TIME;
        fut1 = runAsync([fileNames]() { ChildrenNamesFile().read(fileNames); });
        fut1.get();
        STOP_TIME;
        PRINT_DURATION_TIME("First thread: ");

        START_TIME;
        ChildrenRelations().read(argv[2]);
        STOP_TIME;
        PRINT_DURATION_TIME("Second thread: ");

        totalTime.stop();
        print("Total: ", totalTime.duration(), newLine);
    }
}*/

void all()
{
    readChildrenNames();
    readChildrenRelations();
    unlovedChildren();
    unhappyChildren();
    favoriteChildren();
    // concurrencyReading();
}
} // namespace Test
