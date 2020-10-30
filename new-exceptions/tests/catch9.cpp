// expected: 1
#include "common.hpp"

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
