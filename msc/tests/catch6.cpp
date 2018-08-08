// expected: 254
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw MoveThrowsObj(0);
        }
        catch (MoveThrowsObj) {
            return 4;
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
