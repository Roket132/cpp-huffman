cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
    set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O3 -fprofile-arcs -ftest-coverage -pedantic")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif ()

add_executable(myTest
        gtest/gtest.h
        gtest/gtest-all.cc
        gtest/gtest_main.cc
        test.cpp)

add_executable(huffman main.cpp)

include_directories(libhuffman)
add_subdirectory(libhuffman)

target_link_libraries(huffman libhuffman)

target_link_libraries(myTest libhuffman -fuse-linker-plugin)
