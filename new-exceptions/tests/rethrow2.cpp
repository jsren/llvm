// expected: 1
#include "common.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw TrivialObj{57};
        }
        catch (...) {
            __builtin_rethrow(__builtin_get_exception_obj(0));
        }
    }
    catch (int) {
        return 2;
    }
    catch (TrivialObj o) {
        return o.i == 57 ? 1 : 3;
    }
}
