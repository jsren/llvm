// expected: 3
/*
A handler is a match for an exception object of type E if
The handler is of type cv T or cv T& and E and T are the same type (ignoring the top-level cv-qualifiers)
*/
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw 3;
    }
    catch (const volatile int& i) {
        return i;
    }
}
