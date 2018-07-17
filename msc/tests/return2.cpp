// expected: 1
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        try {
            return 1;
        }
        catch (...) {
            return 3;
        }
    }
    catch (...) {
        return 2;
    }
}
