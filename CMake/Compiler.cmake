cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0076 NEW)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if (UNIX)
    # -Weffc++ is too verbose, not usable
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wall -Wextra -Wno-unknown-pragmas -O0 -ggdb -fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fstack-protector")
    link_libraries(stdc++fs)
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL /GS /Gs /O2")
endif()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(PRECOMPILED_HEADERS
        # System headers
        <assert.h> <errno.h> <net/if_arp.h>  <netinet/in.h> <pthread.h> <signal.h> <string.h> <sys/ioctl.h> <sys/socket.h> <sys/types.h>
        # STL headers
        <algorithm> <any> <array> <atomic> <cassert> <cerrno> <chrono> <condition_variable> <cstdarg> <cstdint> <cstdlib>
        <cstring> <ctime> <deque> <exception> <fstream> <functional> <future> <iomanip> <iostream> <istream> <limits>
        <locale> <map> <memory> <mutex> <optional> <ostream> <queue> <random> <regex> <set> <sstream> <stdexcept>
        <string> <string_view> <system_error> <thread> <tuple> <typeindex> <typeinfo> <type_traits> <unordered_map>
        <unordered_set> <utility> <variant> <vector>)

set(ALL_SOURCES_GLOBAL "" CACHE INTERNAL "")

function(add_sources_to_list)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    list(TRANSFORM ARG_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
    set(ALL_SOURCES_GLOBAL ${ALL_SOURCES_GLOBAL} ${ARG_SOURCES} CACHE INTERNAL "")
endfunction(add_sources_to_list)

function(setup_target)
    set(oneValueArgs TARGET)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    message("Setting up target " ${ARG_TARGET})
    target_precompile_headers(${ARG_TARGET} PRIVATE ${PRECOMPILED_HEADERS})
    set_property(TARGET ${ARG_TARGET} PROPERTY CXX_STANDARD 20)
    target_link_libraries(${ARG_TARGET} ${Boost_LIBRARIES} Threads::Threads)
    target_include_directories(${ARG_TARGET} PRIVATE ${Boost_INCLUDE_DIRS})
endfunction()