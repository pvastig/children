#pragma once
#ifndef _utils_h_
#define _utils_h_

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
    std::cout << NEWLINE;
    for (const auto& item : container)
      printArgs(item, NEWLINE);
  }

  template<typename Any>
  void printDataMap(const Any& container)
  {
    for (const auto& item : container)
      printArgs(item.first, ' ', item.second, NEWLINE)
  }
}
#endif // _utils_h_
