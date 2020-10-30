// expected: 1
#include "common.hpp"

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
