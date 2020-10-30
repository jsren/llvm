// expected: 254
#include "common.hpp"

int f(int i) throws {
    if (i == 0) throw 0;
    return i;
}

class C {
    int i;
public:
    C(int i, int& b) throws
    try : i(f(i)) {
        b = 3;
    } catch (...) {
        b = 2;
    }
};

int main()
{
    test_setup();

    int out = 0;
    C(0, out);
    return out;
}
