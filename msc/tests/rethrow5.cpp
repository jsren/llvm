// expected: 2
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        try {
            throw SuperObj();
        }
        catch (...) {
            throw;
        }
    }
    catch (const BaseObj& o) {
        return o.value();
    }
}
