#include <iostream>
#include "test.h"

using namespace std;

int main()
{
    cout << "Hello! This is IOU!\n"
         << "---------------------\n"
         << endl;

    testSquare(50,400,400);

    testConvexQuad(50,400,400);

    cout << "---------------------\n"
         << "I'm Done! Thank You\n"
         << endl;

    return 0;
}

