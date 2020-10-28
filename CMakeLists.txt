# Basic CMake setup
cmake_minimum_required(VERSION 3.0.0)
project(odin VERSION 0.1.0 LANGUAGES C)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


# Setup odin options
set(odin_enable_tests ON CACHE BOOL "If tests are going to be compiled or not")


add_subdirectory(source)