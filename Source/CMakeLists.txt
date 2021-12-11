cmake_minimum_required(VERSION 3.21)

find_package(Boost)

set(CPP_UTILS_TARGET "cpp-utils")

add_library(${CPP_UTILS_TARGET} STATIC)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_include_directories(${CPP_UTILS_TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${CPP_UTILS_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries(${CPP_UTILS_TARGET} LINK_PUBLIC ${Boost_LIBRARIES})

add_subdirectory(Utility)
