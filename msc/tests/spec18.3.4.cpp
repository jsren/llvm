// expected: 3
/*
The handlers for a try block are tried in order of appearance.
*/
#include "stdexcept.hpp"

int main()
{
    test_setup();
    try {
        throw SuperObj();
    }
    catch (BaseObj) {
        return 3;
    }
    catch (SuperObj) {
        return 2;
    }
}
