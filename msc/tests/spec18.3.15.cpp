// expected: 2
/*
The variable declared by the exception-declaration, of type cv T or cv T&, is initialized from the exception
object, of type E, as follows
*/
#include "stdexcept.hpp"

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
