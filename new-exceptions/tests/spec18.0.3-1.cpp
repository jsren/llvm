// expected: 1
/*
A goto, break, return, or continue statement can be used to transfer control out of a try
block or handler. When this happens, each variable declared in the try block will be destroyed in the context
that directly contains its declaration.
*/
#include "stdexcept.hpp"

int main()
{
    test_setup();
    int c = 0;
    try {
        DtorTestObj o{c};
        goto end;
    }
    catch (...) {
        return 2;
    }
end:
    return c;
}
