// expected: 3
/*
A handler is a match for an exception object of type E if
the handler is of type cv T or cv T& and T is an unambiguous public base class of E
*/
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw SuperObj();
    }
    catch (const volatile BaseObj&) {
        return 3;
    }
}
