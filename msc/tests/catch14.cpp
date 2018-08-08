// expected: 2
#include "stdexcept.hpp"

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
