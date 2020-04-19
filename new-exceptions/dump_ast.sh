#!/bin/bash
CUR_DIR="$(cd -P "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
$CUR_DIR/../../build-llvm/bin/clang++ -Xclang -ast-dump -c "$1"
