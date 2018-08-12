CLANG := /source/build-llvm/bin/clang++
#ARGS := -s -fno-rtti -fzcexceptions -O3 -flto ./spec18.3.11.cpp -std=c++14 -o spec
EXCEPT ?= -fzcexceptions
INCLUDE := -I../tests
ARGS := 


build_main:
	$(CLANG) $(EXCEPT) -s -fno-rtti -Wno-c++17-extensions -static-libstdc++ -std=c++14 $(INCLUDE) -O3 -flto -Wall -Wextra std.cpp bench.cpp main.cpp -o bench.exe
