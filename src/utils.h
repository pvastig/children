#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string_view>

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
    std::ios::sync_with_stdio(false);
    std::cout << std::forward<Any>(first);
    printArgs(args...);
}

template<class Any>
void printContainer(Any&& container)
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
        const auto & names = nameRelation.second;
        if (names.empty())
            continue;

        const auto & firstName = nameRelation.first;
        const auto & firstRelationName = names.begin();
        printArgs(firstName, '\t', *firstRelationName, newLine);
        std::for_each(std::next(names.cbegin()), names.cend(),
                      [](const auto & relationName)
                      {
                          printArgs('\t', relationName, utils::newLine);
                      }
                      );
    }
}

template<class T>
class Singleton
{
public:
    static T & instance()
    {
        static T object;
        return object;
    }
};

class Timer final : public Singleton<Timer>
{
public:
    Timer();
    void start();
    void end();
    long duration() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_end;
};

#define START_TIME    Timer::instance().start()
#define STOP_TIME     Timer::instance().end()
#define DURATION_TIME Timer::instance().duration()

class Log final : public Singleton<Log>
{
public:
    void setFileName(std::string_view fileName);
    Log & operator<<(std::string_view str);
private:
    std::string m_fileName;
};
}
