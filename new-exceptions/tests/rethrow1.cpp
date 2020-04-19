// expected: 254
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw TrivialObj{57};
    }
    catch (...) {
        __builtin_rethrow(__builtin_get_exception_obj(0));
    }
}
