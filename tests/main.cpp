#include "test.h"

#include <iostream>

int main()
{
    try {
        using namespace Test;
        //        readChildrenNames();
        //        readChildrenRelations();
        //        unlovedChildren();
        //        unhappyChildren();
        //        favoriteChildren();
        all();
        // testConcurrencyReading();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
    }
    return 0;
}
