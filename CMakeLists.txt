cmake_minimum_required(VERSION 3.5)

project(readingsconsumer)

option(BUILD_WITH_VCPKG "Build with config" OFF)

if (BUILD_WITH_VCPKG)
    message("Build with VCPKG!!!")
endif()

enable_testing()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_subdirectory(src)
#add_subdirectory(test)
