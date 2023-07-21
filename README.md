# C++ Utilities [![CMake](https://github.com/licekto/cpp-utils/actions/workflows/cmake.yml/badge.svg)](https://github.com/licekto/cpp-utils/actions/workflows/cmake.yml)

Collection of various C++ utilities.

## List of utilities

- FullEnum: C++ enums with `toString`, `fromString` methods, stream operators and other support methods
- Logger
    - supports compile-time disabling of log levels
    - clean interface: `log_debug << "ab" << 1;`
    - timestamp, source location, thread ID
- Preprocessor: C++20 "recursive" macros
- ProgramOptions: Program options parser
- Retriable: Launches a thread with a given operation and retries if the operation has not finished in time
- SimpleLogger: Tiny logging macros for lightweight use
- StringUtils: String concatenation utilities and others
- ThreadSafeQueue: Tiny and simple thread safe queue
- Tokenizer: Non-copy string tokenizer based on `string_view`

## Build

Builds with C++20, GCC and MSVC.

## Tests

Contains unit tests written in Catch2.

## Checks

Supports optional usage of `cppcheck`, `clang-tidy` and `include-what-you-use`.

Generates _test coverage_ report using `lcov`.

## TODO
- multithreaded output stream: copy values to a queue, convert to string and flush in a separate thread
- use ^ in logger as a policy
- add named threads and use in logger
- C++20 modules
- Follow hints on CMake from https://github.com/StableCoder/cmake-scripts
    - Finalize test coverage: add HTML generation
    - Add doxygen comments and generate the documentation
    - Try AFL++ fuzzing
