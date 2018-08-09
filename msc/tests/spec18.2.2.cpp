// expected: 2
/*
The destructor is invoked for each automatic object of class type constructed, but not yet destroyed, since
the try block was entered. If an exception is thrown during the destruction of temporaries or local variables
for a return statement (9.6.3), the destructor for the returned object (if any) is also invoked. The objects
are destroyed in the reverse order of the completion of their construction.
*/
#include "stdexcept.hpp"

auto foo(int& count) throws
{
    DtorThrowsObj o{0};
    DtorTestObj{count};
    return DtorTestObj{count};
}

int main()
{
    test_setup();
    int count = 0;
    try {
        foo(count);
    }
    catch (...) {
        return count;
    }
}
