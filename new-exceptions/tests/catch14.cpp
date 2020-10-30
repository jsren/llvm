// expected: 1
#include "common.hpp"

int main()
{
    test_setup();
    int count = 0;
    try {
        throw DtorTestObj(count);
    }
    catch (...) {
    }
    return count;
}
