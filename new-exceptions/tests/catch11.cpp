// expected: 2
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw SuperObj();
    }
    catch (const BaseObj& o) {
        return o.value();
    }
}
