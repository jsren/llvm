// expected: 254
/*
If no matching handler is found, the function std::terminate() is called; whether or not the stack is
unwound before this call to std::terminate() is implementation-defined (18.5.1).
*/
#include "stdexcept.hpp"

int foo() throws
{
    try {
        throw SuperObj();
    }
    catch (int) {
        return 2;
    }
}

int main()
{
    test_setup();
    try {
        return foo();
    }
    catch (int) {
        return 3;
    }
}
