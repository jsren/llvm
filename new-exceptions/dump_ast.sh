#!/bin/bash

~/build-llvm/bin/clang++ -Xclang -ast-dump -c $1
#lldb-6.0 build-llvm/bin/clang++ -- -Xclang -ast-dump -c $MSC_DIR/ast_test1.cpp