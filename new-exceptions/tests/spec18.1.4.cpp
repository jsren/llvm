// expected: 6
/*
The memory for the exception object is allocated in an unspecified way, except as noted in 6.7.4.1. If a
handler exits by rethrowing, control is passed to another handler for the same exception object.
Among all points of potential destruction for the exception object, there is an unspecified last one where the
exception object is destroyed. All other points happen before that last one (4.7.1).
The implementation may then deallocate the memory for the exception object; any such deallocation is done in an unspecified way.
*/
#include "common.hpp"

[[gnu::weak]] alignas(1)
    char __typeid_for_Test = 0;

struct Test {
    int& value;
    Test(int& value) : value(value) { }
    ~Test() {
        value += 3;
    }
};

int main()
{
    test_setup();
    int out = 0;
    try {
        throw Test(out);
    }
    catch (Test t) {
        // Destroy in buffer
        // Destroy local
    }
    return out;
}
