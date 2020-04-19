// expected: 1
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw EmptyObj();
    }
    catch (int) {
        return 2;
    }
    catch (EmptyObj) {
        return 1;
    }
}
