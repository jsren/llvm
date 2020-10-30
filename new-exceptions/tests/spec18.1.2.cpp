// expected: 2
/*
When an exception is thrown, control is transferred to the nearest handler with a matching type (18.3);
“nearest” means the handler for which the compound-statement or ctor-initializer following the try keyword
was most recently entered by the thread of control and not yet exited.
*/
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw 1;
    }
    catch (int) {
        return 2;
    }
    catch (...) {
        return 1;
    }
    return 0;
}
