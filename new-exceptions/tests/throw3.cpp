// expected: 3
#include "common.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw CtorThrowsObj(0);
        }
        catch (int) {
            return 3;
        }
    }
    catch (int) {
        return 2;
    }
    catch (CtorThrowsObj) {
        return 1;
    }
}
