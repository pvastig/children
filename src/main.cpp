#include "data.h"

#include <iostream>

int main(int argc, char const* argv[]) {
  try {
    pa::ProcessDataFacade(argc, argv).run();
    return 0;
  } catch (std::invalid_argument const& e) {
    std::cerr << "Invalid argument: " << e.what();
  } catch (std::bad_alloc const& e) {
    std::cerr << "Bad allocation: " << e.what();
  } catch (std::exception const& e) {
    std::cerr << "Exception: " << e.what();
  } catch (...) {
    std::cerr << "Unknown error";
  }

  std::cout << std::endl;
  return 1;
}
