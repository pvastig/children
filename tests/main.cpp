#include "test.h"

#include <iostream>

int main()
{
    try
    {
        //Test::readDataNames();
        //Test::readDataRelations();
        //Test::unlovedChildren();
        //Test::unhappyChildren();
        //Test::favouriteChildren();
        Test::all();
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
