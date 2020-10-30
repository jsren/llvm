// expected: 1
#include "common.hpp"

int main()
{
    test_setup();

    EmptyObj o{};
    EmptyObj o2{static_cast<EmptyObj&&>(o)};

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
