# Basic CMake setup
cmake_minimum_required(VERSION 3.0.0)
project(odin VERSION 0.1.0 LANGUAGES C)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


# Setup odin options
set(odin_enable_examples ON CACHE BOOL "If examples are going to be compiled or not")
set(odin_use_aether_math ON CACHE BOOL "If odin is going to use the default config for mathematics")


set(odin_sources "")

# Build the sources list
list(APPEND odin_sources 
    "source/o_api.c"
)

# Find Third Party libraries

    # Find Vulkan
    find_package(Vulkan REQUIRED)

    if(Vulkan_FOUND)

        # Add Vulkan API Sources
        

    endif()



add_library(odin STATIC ${odin_sources})


# Link, include, and define Third Party libraries

    # Find Vulkan
    if(Vulkan_FOUND)
        # Link
        target_link_libraries(odin ${Vulkan_LIBRARIES})        
        
        # Include
        target_include_directories(odin PUBLIC ${Vulkan_INCLUDE_DIRS})
    
        # Define
        target_compile_definitions(odin PUBLIC ODIN_HAS_VULKAN)
    endif()




target_include_directories(odin PUBLIC include)