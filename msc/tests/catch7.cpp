// expected: 1
#include "stdexcept.hpp"

int main()
{
    SuperObj so(SuperObj{});
    test_setup();
    try {
        throw SuperObj();
    }
    catch (BaseObj) {
        return 1;
    }
    catch (...) {
        return 2;
    }
}
