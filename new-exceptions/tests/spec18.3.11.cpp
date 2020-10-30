// expected: 3
/*
The scope and lifetime of the parameters of a function or constructor extend into the handlers of a function-
try-block.
*/
#include "common.hpp"

struct Test {
    Test(DtorTestObj o, int& out) throws try {
        throw 1;
    }
    catch (...) {
        out = o.i == 0 ? 5 : 4;
    }
};

int main()
{
    test_setup();

    int count = 0;
    int out = 0;
    try {
        Test(DtorTestObj(count), out);
    }
    catch (...) {
        return out == 5 && count == 1 ? 3 : 4;
    }
}
