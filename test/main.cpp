#include <iostream>
#include "test.h"

int main()
{
    std::cout << "Hello! This is IOU!\n"
              << "---------------------\n"
              << std::endl;

    int n1 = testSquare(50, 400, 400);
    n1 += testSquare(1000, 400, 400, false);

    int n2 = testPlygon(4, 50, 400, 400);
    for (int i=4; i<30; ++i)
        n2 += testPlygon(i, 1000, 400, 400, false);


    std::cout << std::endl
              << n1 << "+" << n2 << "failed."
              << std::endl;

    std::cout << "---------------------\n"
              << "I'm Done! Thank You\n"
              << std::endl;

    return 0;
}

