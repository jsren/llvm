// expected: 2
/*
The currently handled exception is rethrown if control reaches the end of a handler of the function-try-block
of a constructor or destructor. Otherwise, flowing off the end of the compound-statement of a handler of a
function-try-block is equivalent to flowing off the end of the compound-statement of that function (see 9.6.3).
*/
#include "common.hpp"

struct Test {
    Test() throws try {
        throw 1;
    }
    catch (...) {
    }
};

int main()
{
    test_setup();
    try {
        Test();
    }
    catch (int) {
        return 2;
    }
    catch (...) {
        return 1;
    }
}
