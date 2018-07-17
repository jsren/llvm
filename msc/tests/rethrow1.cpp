// expected: 254
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw TrivialObj{57};
    }
    catch (...) {
        throw;
    }
}
