// expected: 3
/*
the handler is of type cv T or const T& where T is a pointer or pointer to member type and E is a
pointer or pointer to member type that can be converted to T
*/
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw new SuperObj();
    }
    catch (const volatile BaseObj*&) {
        return 3;
    }
}
