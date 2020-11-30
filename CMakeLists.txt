# Basic CMake setup
cmake_minimum_required(VERSION 3.0.0)
project(odin VERSION 0.1.0 LANGUAGES C CXX)

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
        list(APPEND odin_sources
            "source/apis/vulkan/o_vulkan_api.c"
            "source/apis/vulkan/o_vulkan_initialize.c"
            "source/apis/vulkan/o_vulkan_vma.cpp"
            "source/apis/vulkan/o_vulkan_physical_device.c"
            "source/apis/vulkan/o_vulkan_vertex_assembly.c"
            "source/apis/vulkan/o_vulkan_vertex_buffer.c"
            "source/apis/vulkan/o_vulkan_index_buffer.c"
            "source/apis/vulkan/o_vulkan_render_pass.c"
            "source/apis/vulkan/o_vulkan_pipeline.c"
            "source/apis/vulkan/o_vulkan_texture_2d.c"
            "source/apis/vulkan/o_vulkan_framebuffer.c"
        )

        # Add vulkan platform specific sources
        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

            list(APPEND odin_sources
                "source/apis/vulkan/platforms/o_vulkan_platform_windows.c"
                "source/apis/vulkan/o_vulkan_window_windows.c"
            )

        elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

        endif()

    endif()

# Add COAST Entertainment required libraries
add_subdirectory("source/aero")


add_library(odin STATIC ${odin_sources})


# Link COAST Entertainment libraries
target_link_libraries(odin aero)

# Link, include, and define Third Party libraries

    # Find Vulkan
    if(Vulkan_FOUND)
        # Link
        target_link_libraries(odin ${Vulkan_LIBRARIES})        
        
        # Add vulkan platform specific links
        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            target_link_libraries(odin dwmapi.dll)
        endif()

        # Include
        target_include_directories(odin PUBLIC ${Vulkan_INCLUDE_DIRS})

        # Define
        target_compile_definitions(odin PUBLIC ODIN_HAS_VULKAN)

            # Add vulkan platform specific defines
        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            target_compile_definitions(odin PUBLIC VK_USE_PLATFORM_WIN32_KHR)
        endif()

    endif()

target_include_directories(odin PUBLIC include)


# Build Examples
if(odin_enable_examples)

    add_subdirectory(examples)

endif()