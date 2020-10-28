
# Build required libs
add_subdirectory(aether)

# Define a list that has no values
set(OdinSources "")

# Add all the basic odin source files
list(APPEND OdinSources
    "odin.h"
    "o_api.h"
    "o_api.c"
    "o_data.h"
    "o_physical_device.h"
    "o_platform.h"
    "o_platform_data.h"

    "o_vertex_assembly.h"

    "o_vertex_buffer.h"
    "o_index_buffer.h"
    "o_uniform_buffer.h"
    "o_command_buffer.h"

    "o_pipeline.h"

    "o_draw.h"
)



# Vulkan

find_package(Vulkan REQUIRED)

if(Vulkan_FOUND)

    list(APPEND OdinSources
                
    )

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

        list(APPEND OdinSources
            "vulkan/platforms/o_vulkan_platform_windows.c"
        )

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    
        list(APPEND OdinSources
            "vulkan/platforms/o_vulkan_platform_linux.c"
        )

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

        list(APPEND OdinSources
            "o_vulkan_platform_darwin.c"
        )

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")

    list(APPEND OdinSources
        "o_vulkan_platform_android.c"
    )

    endif()

endif()



# Create Library
add_library(Odin STATIC ${OdinSources})

target_link_libraries(Odin freetype)


# Odin Definitions

if(Vulkan_FOUND)
    

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

        target_compile_definitions(Odin PUBLIC ODIN_VULKAN VK_USE_PLATFORM_WIN32_KHR)

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

        target_compile_definitions(Odin PUBLIC ODIN_VULKAN VK_USE_PLATFORM_XLIB_KHR)

    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")



    elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")



endif()

endif()


target_include_directories(Odin PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Odin ${Vulkan_LIBRARIES})


if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

    # For extra window options
    target_link_libraries(Odin dwmapi.dll)

endif()