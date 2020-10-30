// expected: 1
/*
Throwing an exception transfers control to a handler. An object is passed and the type of that object determines which handlers can catch it.
*/
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw 1;
    }
    catch (int i) {
        return i;
    }
}
