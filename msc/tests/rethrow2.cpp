// expected: 1
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw TrivialObj{57};
        }
        catch (...) {
            throw;
        }
    }
    catch (int) {
        return 2;
    }
    catch (TrivialObj o) {
        return o.i == 57 ? 1 : 3;
    }
}
