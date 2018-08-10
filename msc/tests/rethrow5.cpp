// expected: 2
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw SuperObj();
        }
        catch (...) {
            __exception_obj_base* ex = __builtin_get_exception_obj(0);
            __builtin_rethrow(ex);
        }
    }
    catch (const BaseObj& o) {
        return o.value();
    }
}
