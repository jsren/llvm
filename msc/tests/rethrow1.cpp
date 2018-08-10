// expected: 254
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw TrivialObj{57};
    }
    catch (...) {
        __exception_obj_base* ex = __builtin_get_exception_obj(0);
        __builtin_rethrow(ex);
    }
}
