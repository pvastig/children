#include "stdafx.h"

#include "data.h"
#include "test.h"

int main(int argc, char * argv[])
{
  try
  {
     PavelA::ProcessData prData(argc, argv);
     prData.run();
    //Test::printArgs();
    //Test::readDataNames();
    //Test::readDataRelations();
    //Test::unlovedChildrenList();
    //Test::unhappyChildrenList();
    return 0;
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << "Invalid argument: " << e.what();
    return 1;
  }
  catch (const std::bad_cast& e)
  {
    std::cerr << "bad_cast: " << e.what();
    return 1;
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "bad_alloc: " << e.what();
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << "exception: " << e.what();
    return 1;
  }
  catch (...)
  {
    std::cerr << "Unknown error";
    return 1;
  }
}
