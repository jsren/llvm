## Deterministic exceptions for C++

This repository contains a fork of clang/llvm with support for
"deterministic exceptions".

Determinstic exceptions are a new C++ exception implementation
better suited for embedded and freestanding applications which
aims to achieve drastically smaller binaries, better enable static
analyis, and to be _much_ simpler to implement.

See the [paper](https://www.research.ed.ac.uk/portal/files/78829292/low_cost_deterministic_C_exceptions_for_embedded_systems.pdf)
for details about the implementation.

See the [contribution guide](contributing.md) for an overview of the changes made to clang-7.

## Building

This repository is already set up to build clang with deterministic exception
support.

Given clang's memory-intensive link step, it's best to limit the number of parallel
linker jobs that can be run at once. Set `LLVM_PARALLEL_LINK_JOBS` to an appropriate
number for your machine.

``` bash
git clone <repository> llvm
mkdir build-llvm && cd build-llvm

# Configure
cmake -DLLVM_TARGETS_TO_BUILD="X86;ARM" -DLLVM_PARALLEL_LINK_JOBS=2 -G "Ninja" ../llvm

# Build
ninja
```

## Running

To enable deterministic exceptions, use the `-fzcexceptions` flag.

```bash
build-llvm/bin/clang++ -std=c++17 -fzcexceptions llvm/new-exceptions/tests/catch1.cpp
```

## Testing

To test changes to the deterministic execptions implementation, run the following:

```bash
new-exceptions/tests/run.py
```

If you have built clang in a different directory from the one in the [building](#building) section,
use the `CLANGXX_BINARY` environment variable to point to your `clang++`.

## LLVM readme

The LLVM Compiler Infrastructure
================================

This directory and its subdirectories contain source code for LLVM,
a toolkit for the construction of highly optimized compilers,
optimizers, and runtime environments.

LLVM is open source software. You may freely distribute it under the terms of
the license agreement found in LICENSE.txt.

Please see the documentation provided in docs/ for further
assistance with LLVM, and in particular docs/GettingStarted.rst for getting
started with LLVM and docs/README.txt for an overview of LLVM's
documentation setup.

If you are writing a package for LLVM, see docs/Packaging.rst for our
suggestions.
