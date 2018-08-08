// expected: 55
/*
A goto, break, return, or continue statement can be used to transfer control out of a try
block or handler. When this happens, each variable declared in the try block will be destroyed in the context
that directly contains its declaration.
*/
#include "stdexcept.hpp"

int main()
{
    test_setup();
    bool once = false;
    int count = 0;
lab:
    if (count != 0) return count;
    try {
        DtorThrowsObj o{55};
        try {
            DtorTestObj o{count};
            goto lab;
        }
        catch(...) {
            return 77;
        }
    }
    catch(...) {
        return count == 1 ? 55 : 66;
    }
}
