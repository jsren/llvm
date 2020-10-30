#include "common.hpp"
#include <cstdio>

struct Base
{
    ~Base() {
        std::printf("Destroy base.\n");
    }
};

struct SubObj
{
    ~SubObj() {
        std::printf("Destroy subobj.\n");
    }
};

struct Test : public Base {
    SubObj obj{};

    Test() throws
    {
        throw nullptr;
    }
};

int main()
{
    try {
        Test();
    }
    catch (...) {

    }
}
