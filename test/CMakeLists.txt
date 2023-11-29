cmake_minimum_required(VERSION 3.20.0)

project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

set (HOUR_TEST_SOURCES
    tst_hour.cpp
    ../src/providers/hour.h
)

add_executable(tst_hour ${HOUR_TEST_SOURCES})
add_test(NAME tst_hour COMMAND tst_hour)
target_include_directories(tst_hour PRIVATE ../src/providers)
target_link_libraries(tst_hour PRIVATE gtest gtest_main pthread)

set (ACCUMULATEDPOWER_TEST_SOURCES
    tst_accumulatedpower.cpp
    ../src/consumers/accumulatedpower.cpp
)

add_executable(tst_accumulatedpower ${ACCUMULATEDPOWER_TEST_SOURCES})
add_test(NAME tst_accumulatedpower COMMAND tst_accumulatedpower)
target_include_directories(tst_accumulatedpower PRIVATE ../src/consumers)
target_link_libraries(tst_accumulatedpower PRIVATE gtest gtest_main gmock pthread jsoncpp)
