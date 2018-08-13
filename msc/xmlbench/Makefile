CLANG := /source/build-llvm/bin/clang++
#ARGS := -s -fno-rtti -fzcexceptions -O3 -flto ./spec18.3.11.cpp -std=c++14 -o spec
EXCEPT ?= -fzcexceptions -fno-unwind-tables
INCLUDE := -I../tests
CXX_ARGS := -fno-rtti -s -Wno-c++17-extensions -std=c++14 $(INCLUDE) -O3 -flto -Wall -Wextra -static-libstdc++ -static-libgcc

SYS_INCLUDES := -isystem /usr/arm-linux-gnueabihf/include -isystem /usr/arm-linux-gnueabihf/include/c++/6 -isystem /usr/arm-linux-gnueabihf/include/c++/6/arm-linux-gnueabihf/
SYS_LIBS := -L/usr/arm-linux-gnueabihf/lib -L/usr/lib/gcc-cross/arm-linux-gnueabihf/7/

build_arm:
	$(CLANG) $(EXCEPT) -target arm-linux-gnueabihf  $(SYS_INCLUDES) $(SYS_LIBS) $(CXX_ARGS) std.cpp bench.cpp main.cpp -o benchzc.exe
	$(CLANG) -fexceptions -target arm-linux-gnueabihf  $(SYS_INCLUDES) $(SYS_LIBS) $(CXX_ARGS) std.cpp bench.cpp main.cpp -o bench.exe

build_x86:
	$(CLANG) $(EXCEPT) -s -fno-rtti -Wno-c++17-extensions -static-libstdc++ -static-libgcc -std=c++14 $(INCLUDE) -O3 -flto -Wall -Wextra std.cpp bench.cpp main.cpp -o benchzc.exe
	$(CLANG) -fexceptions -s -fno-rtti -Wno-c++17-extensions -static-libstdc++ -static-libgcc -std=c++14 $(INCLUDE) -O3 -flto -Wall -Wextra std.cpp bench.cpp main.cpp -o bench.exe
