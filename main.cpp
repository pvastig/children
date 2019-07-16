#include "data.h"
#include "test.h"

#include <iostream>

int main(int argc, char * argv[])
{
  std::ios::sync_with_stdio(false);
  try
  {
    //pa::ProcessDataFacade prData(argc, argv);
    //prData.run();
    //Test::printArgs();
    //Test::readDataNames();
    //Test::readDataRelations();
    //Test::unlovedChildren();
    //Test::unhappyChildren();
    //Test::favouriteChildren();
    //Test::all();
    return 0;
  }
  catch (std::invalid_argument const & e)
  {
    std::cerr << "Invalid argument: " << e.what();
  }
  catch (std::bad_alloc const & e)
  {
    std::cerr << "bad_alloc: " << e.what();
  }
  catch (const std::exception& e)
  {
    std::cerr << "exception: " << e.what();
  }
  catch (...)
  {
    std::cerr << "Unknown error";
  }

  std::cout << std::endl;
  return 1;
}

