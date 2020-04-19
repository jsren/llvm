#!/bin/bash
CUR_DIR="$(cd -P "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
$CUR_DIR/../../build-llvm/bin/clang++ -c -target arm-none-eabi -isystem /usr/lib/arm-none-eabi/newlib/armv7-m -isystem /usr/lib/arm-none-eabi/newlib -isystem /usr/include/newlib/c++/6.3.1 -isystem /usr/include/newlib/c++/6.3.1/arm-none-eabi -isystem /usr/include/newlib "$1"
