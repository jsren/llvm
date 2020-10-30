// expected: 2
/*
A handler will be
invoked only by throwing an exception in code executed in the handler’s try block or in functions called from
the handler’s try block.
*/
#include "common.hpp"

int main()
{
    test_setup();
    try {
    }
    catch (...) {
        return 1;
    }
    return 2;
}
