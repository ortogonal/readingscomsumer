cmake_minimum_required(VERSION 3.5)

project(readingsconsumer)

enable_testing()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_subdirectory(src)
add_subdirectory(test)
