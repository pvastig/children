#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <string_view>
#include <type_traits>

namespace utils {
constexpr char const* newLine = "\n";
#define PRINT_DASHED_LINE std::cout << "--------------" << std::endl

// TODO: improve template. leave comment about working
template<class T>
void printComplexContainer(T container)
{
    for (auto const& [key, items] : container) {
        auto const& firstValue = *items.cbegin();
        print(key, '\t', firstValue, newLine);
        std::for_each(std::next(items.cbegin()), items.cend(), [](auto const& item) {
            print('\t', item, newLine);
        });
    }
}

template<class T>
class Singleton final
{
public:
    Singleton& operator=(Singleton) = delete;
    Singleton(Singleton const&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T& instance()
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

#define START_TIME Singleton<Timer>::instance().start()
#define STOP_TIME Singleton<Timer>::instance().stop()
#define DURATION_TIME Singleton<Timer>::instance().duration()

class Log
{
public:
    Log(std::string_view fileName);
    ~Log();
    template<class T>
    Log& operator<<(T msg);

private:
    std::ofstream m_outputFile;
};

template<class T>
Log& Log::operator<<(T msg)
{
    if (m_outputFile.is_open())
        m_outputFile << msg;
    return *this;
}

template<class F, typename... Ts>
auto runAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

// templates for determining whether the std::pair container element is
template<typename T>
struct isPair : std::false_type
{};

template<typename T, typename U>
struct isPair<std::pair<T, U>> : std::true_type
{};

template<typename T>
constexpr bool isPairV = isPair<T>::value;

// templates for determining if the container is std::map
template<typename, typename = void>
struct isMapping : std::false_type
{};

template<typename Container>
struct isMapping<
    Container,
    std::enable_if_t<isPairV<typename std::iterator_traits<typename Container::iterator>::value_type>>>
    : std::true_type
{};

template<typename T>
constexpr bool isMappingV = isMapping<T>::value;
template<class Out, class Container>
typename std::enable_if_t<isMappingV<Container>, void> print(Out& out, Container&& container);

template<class Out, class Container>
typename std::enable_if_t<!isMappingV<Container>, void> print(Out& out, Container&& container);

template<class Out, typename... Args>
void print(Out& out, Args... args)
{
    (out << ... << args);
}

template<class Out, class Container>
typename std::enable_if_t<!isMappingV<Container>> print(Out& out, Container&& container)
{
    for (auto&& item : std::forward<Container>(container))
        out << item << newLine;
}

template<class Out, class Container>
typename std::enable_if_t<isMappingV<Container>, void> print(Out& out, Container&& container)
{
    for (auto&& [first, second] : std::forward<Container>(container))
        out << first << " " << second;
}
} // namespace utils
