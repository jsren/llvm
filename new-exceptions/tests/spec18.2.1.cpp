// expected: 3
/*
As control passes from the point where an exception is thrown to a handler, destructors are invoked by a
process, specified in this section, called stack unwinding.
*/
#include "common.hpp"

void foo(int& count) throws
{
    DtorTestObj o{count};
    try {
        DtorTestObj o1{count};
        throw 1;
    }
    catch (TrivialObj) {
    }
}

int main()
{
    test_setup();
    int count = 0;
    try {
        DtorTestObj o2{count};
        foo(count);
    }
    catch (...) {
        return count;
    }
}
