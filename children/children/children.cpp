#include "stdafx.h"

#include "data.h"
#include "test.h"

int main(int argc, char * argv[])
{
  try
  {
    enum class eUserSelect : char
    {
      eExit = 0,
      eUnlovedChildrenNames,
      eUnhappyChildrenNames,
      eFavouriteChildrenNames,
      ePrintData
    };

    PavelA::ProcessData prData(argc, argv);

    const std::string menu = "\nPlease, select action:\n"
      "1 - unloved children\n"
      "2 - unhappy children\n"
      "3 - favorite children\n"
      "4 - print loaded data\n"
      "------------------------\n"
      "0 - exit\n";

    int num = 0;
    bool readAgain = true;
    do
    {
      std::cout << menu << std::endl;
      std::cin >> num;
      if (!std::cin)
      {
        std::cin.clear();
        std::string s;
        std::cin >> s;
        std::cout << "You entered " << '"' << s << '"' << " please, try again" << std::endl;;
      }
      switch ((eUserSelect)num)
      {
        case eUserSelect::eUnlovedChildrenNames:
        {
          PavelA::printData(prData.unlovedChildrenNames());
          break;
        }
        case eUserSelect::eUnhappyChildrenNames:
        {
          PavelA::printData(prData.unhappyChildrenNames());
          break;
        }
        case eUserSelect::eFavouriteChildrenNames:
        {
          PavelA::printData(prData.favouriteChildrenNames());
          break;
        }
        case eUserSelect::ePrintData:
        {
          //it needs to improve
/*          const PavelA::DataPtrArray & aData = prData.getDataPtrArray();
          for (const auto & data : aData)
            data->printData();*/
          break;
        }
        case eUserSelect::eExit:
        {
          std::cout << "Bye-bye :)" << std::endl;
          readAgain = false;
          break;
        }
        default:
          std::cout << "You entered not existed action, please, try again" << std::endl;
          break;
      }

    } while (readAgain);

    return 0;
  }
  catch (std::invalid_argument & e)
  {
    std::cerr << "Invalid argument: " << e.what();
    return 1;
  }
  catch (std::bad_cast & e)
  {
    std::cerr << "bad_cast: " << e.what();
    return 1;
  }
  catch (std::bad_alloc & e)
  {
    std::cerr << "bad_alloc: " << e.what();
    return 1;
  }
  catch (std::exception & e)
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
