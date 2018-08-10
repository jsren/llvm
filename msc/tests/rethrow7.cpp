// expected: 2
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw SuperObj();
        }
        catch (BaseObj o) {
            __builtin_rethrow(__builtin_get_exception_obj(0));
        }
    }
    catch (SuperObj o) {
        return o.value();
    }
}
