// expected: 1
#include "common.hpp"

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
