// expected: 4
#include "common.hpp"

// TODO: handle typedef for function pointer
// TODO: lamdas?

// If we declare a funcptr typedef within extern "C++"
// then it needs to be converted??
// Check const func ptr

struct container {
    void (*func_ptr1)() throws;
    void (*func_ptr2)() throws;
};

void example1() noexcept(false) {
    throw TrivialObj{57};
}
void example2() noexcept {
    throw TrivialObj{56};
}
void example3() {
    throw TrivialObj{56};
}
void example4() throw() {
    throw TrivialObj{56};
}
void example5() throw(int) {
    throw TrivialObj{56};
}
extern "C++" {
    void example6() noexcept(false) {
        throw TrivialObj{57};
    }
}
extern "C" {
    void example7() {
        throw TrivialObj{56};
    }
}


int main()
{
    test_setup();

    container ctn{};
    ctn.func_ptr1 = example1;
    ctn.func_ptr2 = example2;

    try {
        ctn.func_ptr1();
    }
    catch (TrivialObj o) {
        return o.i == 57 ? 4 : 2;
    }
}
