// expected: 254
#include "common.hpp"

struct container {
    void (*func_ptr1)() throws;
    void (*func_ptr2)();
};

void example1() throws {
    throw TrivialObj{57};
}
void example2() noexcept {
    throw TrivialObj{56};
}

int main()
{
    test_setup();

    container ctn{};
    ctn.func_ptr1 = example1;
    ctn.func_ptr2 = example2;

    try {
        ctn.func_ptr2();
    }
    catch (TrivialObj o) {
        return o.i == 57 ? 4 : 2;
    }
}
