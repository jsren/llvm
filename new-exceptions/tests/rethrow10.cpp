// expected: 57
#include "common.hpp"

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
    catch (TrivialObj o) {
        return o.i;
    }
}
