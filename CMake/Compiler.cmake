cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0076 NEW)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
    # -Weffc++
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wall -Wextra -Wno-unknown-pragmas -O0 -ggdb -fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fstack-protector")
    link_libraries(stdc++fs)
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL /GS /Gs /O2")
endif()

set(PRECOMPILED_HEADERS
        # System headers
        <assert.h> <errno.h> <net/if_arp.h>  <netinet/in.h> <pthread.h> <signal.h> <string.h> <sys/ioctl.h> <sys/socket.h> <sys/types.h>
        # STL headers
        <algorithm> <any> <array> <atomic> <cassert> <cerrno> <chrono> <condition_variable> <cstdarg> <cstdint> <cstdlib>
        <cstring> <ctime> <deque> <exception> <fstream> <functional> <future> <iomanip> <iostream> <istream> <limits>
        <locale> <map> <memory> <mutex> <optional> <ostream> <queue> <random> <regex> <set> <sstream> <stdexcept>
        <string> <string_view> <system_error> <thread> <tuple> <typeindex> <typeinfo> <type_traits> <unordered_map>
        <unordered_set> <utility> <variant> <vector>)