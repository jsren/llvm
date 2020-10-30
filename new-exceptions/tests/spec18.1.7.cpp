// expected: 254
/*
If the exception handling mechanism handling an uncaught exception (18.5.2) directly invokes a function
that exits via an exception, std::terminate is called (18.5.1)
*/
#include "common.hpp"


int main()
{
    test_setup();
    try {
        throw 1;
    }
    catch (TrivialObj) {
        return 2;
    }
}
