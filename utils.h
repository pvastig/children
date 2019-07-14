#pragma once

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

template<typename Any>
void printData(const Any& container)
{
    PRINT_DASHED_LINE;
    for (const auto& item : container)
        printArgs(item, newLine);
    PRINT_DASHED_LINE;
}

template<typename Any>
void printDataMap(const Any& container)
{
    PRINT_DASHED_LINE;
    for (const auto& item : container)
        printArgs(item.first, ' ', item.second, newLine);
    PRINT_DASHED_LINE;
}

static class Timer
{
public:
    Timer()
    {
    }

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
        auto duration = m_end - m_start;
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_start, m_end;
}t;

#define START_TIME    t.start()
#define STOP_TIME     t.end()
#define DURATION_TIME t.duration()
}
