// expected: 57
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw TrivialObj{57};
        }
        catch (...) {
            try {
                throw TrivialObj{33};
            }
            catch (...) {
            }
            __builtin_rethrow(__builtin_get_exception_obj(0));
        }
    }
    catch (TrivialObj o) {
        return o.i;
    }
}