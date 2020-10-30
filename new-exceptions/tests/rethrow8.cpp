// expected: 2
#include "common.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw SuperObj();
        }
        catch (BaseObj o) {
            throw;
        }
    }
    catch (SuperObj o) {
        return o.value();
    }
}
