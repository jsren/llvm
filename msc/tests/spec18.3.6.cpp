// expected: 3
/*
If no match is found among the handlers for a try block, the search for a matching handler continues in a
dynamically surrounding try block of the same thread.
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
    catch (SuperObj) {
        return 3;
    }
}
