#include "test.h"

#include <iostream>

int main()
{
    try
    {
        using namespace Test;
        //readDataNames();
        //readDataRelations();
        //unlovedChildren();
        //unhappyChildren();
        //favouriteChildren();
        all();
        //testLog();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr  << e.what();
    }
    catch (...)
    {
        std::cerr << "Unknown error";
    }
    return 1;
}
