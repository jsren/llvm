#include "common.hpp"
#include <benchmark/benchmark.h>

long long test() throws;
void print(long long i);

static void BM_Profile1(benchmark::State& state)
{
    long long i = 0;
    volatile long long* ptr = &i;

    for (auto _ : state) {
        for (long long n = 0; n < 1000001; n++) {
            try {
                try {
                    //throw EmptyObj();
                    benchmark::DoNotOptimize(i += test());
                    asm("");
                    benchmark::ClobberMemory();
                }
                catch (int) {
                    i += 3;
                }
            }
            catch (int) {
                i += 2;
            }
            catch (EmptyObj) {
                i += 1;
            }
        }
    }
    print(i);
}
BENCHMARK(BM_Profile1);

BENCHMARK_MAIN();
