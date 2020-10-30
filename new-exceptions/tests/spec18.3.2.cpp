// expected: 5
/*
A handler of type “array of T” or function type T is adjusted to be of type “pointer to T”
*/
#include "common.hpp"

int main()
{
    test_setup();
    try {
        throw reinterpret_cast<int*>(0x5);
    }
    catch (int i[]) {
        return (long long)i;
    }
}
