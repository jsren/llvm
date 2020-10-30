// expected: 2
#include "common.hpp"

int main()
{
    try {
        throw new int(3);
    }
    catch (int i) {
        return 1;
    }
    catch (int* n) {
        return 2;
    }
    return 0;
}
