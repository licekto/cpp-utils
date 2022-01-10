# C++ Utilities

Collection of various C++ utilities.

## List of utilities

- ProgramOptions: Program options parser

## Build

Builds with C++17, GCC and MSVC.

## Tests

Contains unit tests written in Catch2.

## TODO
- skipws/noskipws policy in Tokenizer
- multithreaded output stream: copy values to a queue, convert to string and flush in a separate thread
- use ^ in logger as a policy
- multiple sanitizer (asan, ubsan, tsan...) targets for testing using target_compile_definitions
- C++20 modules
- Follow hints on CMake from https://github.com/StableCoder/cmake-scripts
- Add valgrind target