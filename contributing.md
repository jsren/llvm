## Contributing

The GitHub project board tracking ongoing work can be found [here](https://github.com/jsren/llvm/projects/1).

A diff of all of the changes made against the original clang can be found [here](https://github.com/jsren/llvm/compare/d5c1aa4aef2cfa41be7d8ffb1c8c584c4b497985...master).

Non-implementation files specific to the new exception implementation (e.g. tests) can be found under the `new-exceptions` directory.

### Contribution Steps

1. Create or assign an associated GitHub issue on the project board to the contributer.
2. *Work on a separate branch* and create a pull request early on.
3. Any evaluation/investigation results should be added to the GitHub issue.
4. To test changes, follow the steps in the [readme](README.md#testing)

### Implementation Overview

All of the changes to clang/llvm are currently in the clang frontend.
This is in an attempt to be automatically portable and to simplify the implementation.

The bulk of the exception implementation is in `tools/clang/lib/CodeGen/CGException.cpp`.

State specific to the new exception mechanism is generally stored in `tools/clang/include/clang/AST/ASTContext.h`.

A number of new built-in functions have been added in `tools/clang/include/clang/Basic/Builtins.def` and
implemented in `tools/clang/lib/CodeGen/CGBuiltin.cpp` and `tools/clang/lib/Sema/SemaChecking.cpp`

The exception object 'structor thunks are generated in `tools/clang/lib/CodeGen/CodeGenFunction.cpp`.

See the [paper](https://www.research.ed.ac.uk/portal/files/78829292/low_cost_deterministic_C_exceptions_for_embedded_systems.pdf)
for details about the implementation.
