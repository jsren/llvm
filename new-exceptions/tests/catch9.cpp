// expected: 1
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw EmptyObj();
    }
    catch (const EmptyObj&) {
        return 1;
    }
}
