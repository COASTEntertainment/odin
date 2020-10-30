# Basic CMake setup
cmake_minimum_required(VERSION 3.0.0)
project(odin VERSION 0.1.0 LANGUAGES C)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


# Setup odin options
set(odin_enable_examples ON CACHE BOOL "If examples are going to be compiled or not")
set(odin_use_default_mathematics_library ON CACHE BOOL "If odin is going to use the default config for mathematics")

add_subdirectory(source)