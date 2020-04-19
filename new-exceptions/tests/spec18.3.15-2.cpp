// expected: 2
/*
The variable declared by the exception-declaration, of type cv T or cv T&, is initialized from the exception
object, of type E, as follows:
otherwise, the variable is copy-initialized (11.6) from the exception object.
*/
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw SuperObj();
    }
    catch (SuperObj o) {
        return o.value();
    }
}
