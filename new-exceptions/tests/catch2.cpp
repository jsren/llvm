// expected: 1
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw 0;
    }
    catch (...) {
        return 1;
    }
}
