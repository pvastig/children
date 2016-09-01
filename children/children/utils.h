#pragma once

#include <iostream>

namespace PavelA
{
  #define NEWLINE '\n'
}

namespace utils
{
  template<class Any>
  void printArgs(const Any& str)
  {
    std::cout << str;
  }

  template<class Any, typename... Args>
  void printArgs(const Any& first, Args&&... args)
  {
    std::cout << first;
    printArgs(std::forward<Args>(args)...);
  }

  template<typename Any>
  void printData(const Any& container)
  {
    std::copy(container.begin(), container.end(), std::ostream_iterator<Any::value_type>(std::cout, "\n"));
  }

  template<typename Any>
  void printDataMap(const Any& container)
  {
    for (const auto& item : container)
      printArg(item.first, ' ', item.second, PavelA::NEWLINE)
  }
}
