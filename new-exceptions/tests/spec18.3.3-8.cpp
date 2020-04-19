// expected: 3
/*
the handler is of type cv T or const T& where T is a pointer or pointer to member type and E is
std::nullptr_t.
*/
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw nullptr;
    }
    catch (const volatile int*&) {
        return 3;
    }
}
