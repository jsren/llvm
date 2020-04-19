#include "stdexcept.hpp"
#include <cstdio>

extern "C" {
    [[gnu::weak]]
        char __typeid_for_int = 0;
    [[gnu::weak]]
        char __typeid_for_Payload = 0;
}

struct Payload
{
    int i = 42;

    Payload(int o) : i(o) {
        std::printf("Payload constructed\n");
        //std::cout << "Payload constructed\n";
        //*((volatile int*)0x666) = 0xEE;
        /*throw 3;*/
    }
    Payload(const Payload& o) : i(o.i) {
        //*((volatile int*)0x777) = 0xC0;
        std::printf("Payload copy constructed\n");
        //std::cout << "Payload copy constructed with " << i << '\n';
    };
    Payload(Payload&& o) throws : i(o.i) {
        std::printf("Payload move constructed\n");
        //*((volatile int*)0x777) = 0xC0;
        //std::cout << "Payload move constructed\n";
    }
    Payload& operator =(const Payload&) = delete;
    ~Payload() {
        //*((volatile int*)0x888) = 0xFF;
        std::printf("Payload destroyed.\n");
        //std::cout << "Dead Payload contained " << i << '\n';
    }
};

Payload test() throws
{
    if (true) {
        throw Payload(52);
    }
    else return Payload(53);
}
