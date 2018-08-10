// expected: 1
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw TrivialObj{57};
        }
        catch (...) {
            __exception_obj_base* ex = __builtin_get_exception_obj(0);
            __builtin_rethrow(ex);
        }
    }
    catch (int) {
        return 2;
    }
    catch (TrivialObj o) {
        return o.i == 57 ? 1 : 3;
    }
}
