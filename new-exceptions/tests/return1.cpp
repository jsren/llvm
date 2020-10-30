// expected: 1
#include "common.hpp"

int main()
{
    test_setup();
    try {
        return 1;
    }
    catch (...) {
        return 2;
    }
}
