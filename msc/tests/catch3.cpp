// expected: 1
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw 0;
    }
    catch (EmptyObj) {
        return 2;
    }
    catch (...) {
        return 1;
    }
}
