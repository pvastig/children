#pragma once
#ifndef _utils_h_
#define _utils_h_

#include <chrono>

namespace utils
{
  #define NEWLINE "\n"

  template<class Any>
  void printArgs(Any&& str)
  {
    std::cout << std::forward<Any>(str);
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
    std::cout << "\n--------------" << std::endl;
    for (const auto& item : container)
      printArgs(item, NEWLINE);
    std::cout << "--------------" << std::endl;
  }

  template<typename Any>
  void printDataMap(const Any& container)
  {
    std::cout << "\n--------------" << std::endl;
    for (const auto& item : container)
      printArgs(item.first, ' ', item.second, NEWLINE);
    std::cout << "--------------" << std::endl;
  }

  class Time
  {
  public:
    Time()
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
  };

#define START t.start()
#define END   t.end()
#define DURATION t.duration()
}
#endif // _utils_h_
