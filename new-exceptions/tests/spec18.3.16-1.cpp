// expected: 2
/*
When the handler declares an object, any changes to that object will not affect the exception object. When
the handler declares a reference to an object, any changes to the referenced object are changes to the exception
object and will have effect should that object be rethrown.
*/
#include "common.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw SuperObj();
        }
        catch (BaseObj o) {
            o.i = 3;
            __builtin_rethrow(__builtin_get_exception_obj(0));
        }
    }
    catch (const SuperObj& o) {
        return o.i;
    }
}
