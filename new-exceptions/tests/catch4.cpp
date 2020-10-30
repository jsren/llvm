// expected: 1
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw 0;
    }
    catch (EmptyObj) {
        return 2;
    }
    catch (int) {
        return 1;
    }
    catch (...) {
        return 3;
    }
}
