// expected: 12
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw 12;
    }
    catch (const int& i) {
        return i;
    }
}
