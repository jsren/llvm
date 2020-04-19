// expected: 1
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw TrivialObj{57};
    }
    catch (const TrivialObj& o) {
        return o.i == 57 ? 1 : 2;
    }
}
