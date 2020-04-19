#include "stdexcept.hpp"
#include <cstdio>

long long test() throws
{
    return 7;
}

void print(long long i) {
    std::printf("Result: %lld\n", i);
}
