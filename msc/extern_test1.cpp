#include "stdexcept.hpp"
//#include <iostream>

extern "C" {
    [[gnu::weak]]
        char __typeid_for_int = 0;
    [[gnu::weak]]
        char __typeid_for_Payload = 0;
}

struct Payload
{
    int i = 42;

    Payload(int o) throws : i(o) { /*throw 3;*/ }
    Payload(const Payload& o) throws : i(o.i) { };
    Payload& operator =(const Payload&) = delete;
    ~Payload() {
        //std::cout << "Payload contained " << i << '\n';
    }
};

Payload test() throws
{
    if (true) {
        throw Payload(52);
    }
    else return Payload(53);
}
