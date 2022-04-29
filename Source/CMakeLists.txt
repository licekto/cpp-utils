cmake_minimum_required(VERSION 3.21)

add_subdirectory(CppUtils)

if(DEFINED LINK_BOOST)
    set(DUMMY ${LINK_BOOST})
    message("Linking Boost...")
    find_package(Boost)
    target_include_directories(${CPP_UTILS_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
    target_link_libraries(${CPP_UTILS_TARGET} LINK_PUBLIC ${Boost_LIBRARIES})
endif()

set(ALL_SOURCES_LIB "${PROJECT_NAME}" CACHE INTERNAL "")

add_library(${ALL_SOURCES_LIB} STATIC ${ALL_SOURCES_GLOBAL})
setup_target(TARGET ${ALL_SOURCES_LIB})
target_include_directories(${ALL_SOURCES_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})