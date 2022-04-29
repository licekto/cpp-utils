cmake_minimum_required(VERSION 3.21)

set(LOG_LEVEL Info)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LOG_LEVEL Trace)
endif()
configure_file(${PROJECT_SOURCE_DIR}/Source/CppUtils/LogLevel.hpp.in ${CMAKE_BINARY_DIR}/generated/LogLevel.hpp)

set(ALL_SOURCES_GLOBAL "" CACHE INTERNAL "")

function(add_sources_to_list)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    list(TRANSFORM ARG_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
    set(ALL_SOURCES_GLOBAL ${ALL_SOURCES_GLOBAL} ${ARG_SOURCES} CACHE INTERNAL "")
endfunction(add_sources_to_list)

set(PRECOMPILED_HEADERS
        # System headers
        <assert.h> <errno.h> <net/if_arp.h>  <netinet/in.h> <pthread.h> <signal.h> <string.h> <sys/ioctl.h> <sys/socket.h> <sys/types.h>
        # STL headers
        <algorithm> <any> <array> <atomic> <cassert> <cerrno> <chrono> <condition_variable> <cstdarg> <cstdint> <cstdlib>
        <cstring> <ctime> <deque> <exception> <fstream> <functional> <future> <iomanip> <iostream> <istream> <limits>
        <locale> <map> <memory> <mutex> <optional> <ostream> <queue> <random> <regex> <set> <sstream> <stdexcept>
        <string> <string_view> <system_error> <thread> <tuple> <typeindex> <typeinfo> <type_traits> <unordered_map>
        <unordered_set> <utility> <variant> <vector>)

function(setup_target)
    set(oneValueArgs TARGET)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    message("Setting up target " ${ARG_TARGET})
    target_precompile_headers(${ARG_TARGET} PRIVATE ${PRECOMPILED_HEADERS})

    target_include_directories(${ARG_TARGET} PRIVATE ${CMAKE_BINARY_DIR}/generated/)

    set_property(TARGET ${ARG_TARGET} PROPERTY CXX_STANDARD 20)

    target_link_libraries(${ARG_TARGET} Threads::Threads)

    if(proto)
        target_link_libraries(${ARG_TARGET} ${Protobuf_LIBRARIES} proto)
    endif()
    if(cpp-utils)
        target_link_libraries(${ARG_TARGET} cpp-utils)
    endif()

    if(DEFINED LINK_BOOST)
        target_include_directories(${ARG_TARGET} PRIVATE ${Boost_INCLUDE_DIRS})
    endif()
endfunction()