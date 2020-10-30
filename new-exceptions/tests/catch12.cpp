// expected: 2
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw SuperObj();
    }
    catch (const SuperObj& o) {
        return o.value();
    }
    catch (const BaseObj& o) {
        return 1;
    }
}
