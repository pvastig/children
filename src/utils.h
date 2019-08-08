#pragma once

#include <algorithm>
#include <chrono>
#include <future>
#include <fstream>
#include <iostream>
#include <string_view>

namespace utils
{
constexpr char const * newLine = "\n";
#define PRINT_DASHED_LINE std::cout << "--------------" << std::endl

template<class T>
void print(T arg)
{
    std::ios::sync_with_stdio(false);
    std::cout << arg;
}

template<class T, typename... Args>
void print(T firstArg, Args&&... args)
{
    print(firstArg);
    print(args...);
}

template<class Container>
void printContainer(Container&& container)
{
    PRINT_DASHED_LINE;
    for (auto const & item : std::forward<Container>(container))
        print(item, newLine);
    PRINT_DASHED_LINE;
}

//TODO: make overload of template
template<class Map>
void printMap(Map container)
{
    PRINT_DASHED_LINE;
    for (auto const & [first, second] : container)
        print(first, ' ', second, newLine);
    PRINT_DASHED_LINE;
}

//TODO: improve template. leave comment about working
template<class T>
void printComplexContainer(T container)
{
    for (auto const & [key, items] : container)
    {
        auto const & firstValue = *items.cbegin();
        print(key, '\t', firstValue, newLine);
        std::for_each(std::next(items.cbegin()), items.cend(),
                      [](auto const & item)
                      {
                          print('\t', item, newLine);
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

class Timer
{
public:
    void start();
    void stop();
    long duration() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_begin, m_end;
};

#define START_TIME    Singleton<Timer>::instance().start()
#define STOP_TIME     Singleton<Timer>::instance().stop()
#define DURATION_TIME Singleton<Timer>::instance().duration()

class Log
{
public:
    void setFileName(std::string const & fileName);
    void setFileName(std::string_view fileName);
    void enableLog(bool enable);
    template<class T>
    Log & operator<<(T msg);

private:
    std::ofstream m_outputFile;
    std::string m_fileName = "file.log"; //TODO: make date for file name
    bool m_enable = false;
};

template<class T>
Log & Log::operator<<(T msg)
{
    if (!m_enable)
        return *this;
    m_outputFile.open(m_fileName.data(), std::ios::app);
    if(m_outputFile.is_open())
        m_outputFile << msg;
    return *this;
}
#define LOG utils::Singleton<utils::Log>::instance()

template<class F, typename... Ts>
auto runAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}
}

