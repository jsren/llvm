// expected: 3
/*
A ... in a handler’s exception-declaration functions similarly to ... in a function parameter declaration; it
specifies a match for any exception. If present, a ... handler shall be the last handler for its try block.
*/
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw SuperObj();
    }
    catch (...) {
        return 3;
    }
}
