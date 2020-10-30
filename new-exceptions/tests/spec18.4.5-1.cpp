// expected: 254
/*
Whenever an exception is thrown and the search for a handler (18.3) encounters the outermost block of
a function with a non-throwing exception specification, the function std::terminate() is called (18.5.1).
*/
#include "common.hpp"

void nothrow() noexcept {
    throw 1;
}

int main()
{
    test_setup();
    nothrow();
}
