#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>

namespace utils
{
constexpr char const * newLine = "\n";
#define PRINT_DASHED_LINE std::cout << "\n--------------" << std::endl

template<class Any>
void printArgs(Any&& arg)
{
    std::cout << std::forward<Any>(arg);
}

template<class Any, typename... Args>
void printArgs(Any&& first, Args&&... args)
{
    std::cout << std::forward<Any>(first);
    printArgs(args...);
}

template<class Any>
void printContainerData(Any&& container)
{
    PRINT_DASHED_LINE;
    for (auto const & item : std::forward<Any>(container))
        printArgs(item, newLine);
    PRINT_DASHED_LINE;
}

//TODO: make overload of template
template<class Any>
void printDataMap(Any container)
{
    PRINT_DASHED_LINE;
    for (auto const & [first, second] : container)
        printArgs(first, ' ', second, newLine);
    PRINT_DASHED_LINE;
}

//TODO: improve template
template<class Any>
void printComplexContainer(Any container)
{
    for (auto const & nameRelation : container)
    {
        const auto& names = nameRelation.second;
        if (names.empty())
            continue;

        const auto& firstName = nameRelation.first;
        const auto& firstRelationName = names.begin();
        utils::printArgs(firstName, '\t', *firstRelationName, utils::newLine);
        std::for_each(std::next(names.begin()), names.end(),
                      [](const auto& relationName)
                      {
                          utils::printArgs('\t', relationName, utils::newLine);
                      }
                      );
    }
}

static class Timer
{
public:
    Timer() = default;

    void start()
    {
        m_start = std::chrono::system_clock::now();
    }

    void end()
    {
        m_end = std::chrono::system_clock::now();
    }

    auto duration() const
    {
        auto const duration = m_end - m_start;
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_start, m_end;
}t;

#define START_TIME    t.start()
#define STOP_TIME     t.end()
#define DURATION_TIME t.duration()
}
