cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0076 NEW)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if (UNIX)
    # -Weffc++ is too verbose, not usable
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wall -Wextra -Wno-unknown-pragmas -O0 -ggdb -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fstack-protector-strong")
    link_libraries(stdc++fs)
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL /GS /Gs /O2 /W4")
endif()

macro(set_standard)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endmacro()

set_standard()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Too verbose
# -warnings-as-errors=*;
option(CLANG_TIDY "Turns on clang-tidy processing." OFF)
if(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=.;-checks=*;-export-fixes=clang-tidy.yaml;")
endif()

# Generates wrong advices
option(IWYU "Turns on include-what-you-use processing." OFF)
if(IWYU)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE include-what-you-use)
endif()

set(CMAKE_CXX_CPPCHECK "cppcheck;--enable=all;--suppress=missingIncludeSystem;--xml;--output-file=cppcheck_output.txt")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)