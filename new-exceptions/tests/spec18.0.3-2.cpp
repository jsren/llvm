// expected: 2
/*
A goto, break, return, or continue statement can be used to transfer control out of a try
block or handler. When this happens, each variable declared in the try block will be destroyed in the context
that directly contains its declaration.
*/
#include "common.hpp"

int main()
{
    test_setup();
    bool once = false;
    int count = 0;
lab:
    if (count != 0) return count;
    try {
        DtorTestObj o{count};
        try {
            DtorTestObj o{count};
            goto lab;
        }
        catch(...) {
            /* handler 2 */
        }
    }
    catch(...) {
        /* handler 1 */
    }
}
