// expected: 1
/*
The variable declared by the exception-declaration, of type cv T or cv T&, is initialized from the exception
object, of type E, as follows:
if T is a base class of E, the variable is copy-initialized (11.6) from the corresponding base class subobject
of the exception object
*/
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw SuperObj();
    }
    catch (BaseObj o) {
        return o.value();
    }
}
