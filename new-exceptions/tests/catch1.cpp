// expected: 1
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw 1;
    }
    catch (int i) {
        return i;
    }
}
