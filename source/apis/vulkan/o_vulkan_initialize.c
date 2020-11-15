#include "odin/apis/vulkan/o_vulkan_initialize.h"

#include <stdlib.h>

#include <vulkan/vulkan.h>

#include "aero/a_memory.h"
#include "aero/a_string.h"

#include "odin/o_log.h"
#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_platform.h"


/* The debug callback for vulkan. */
VkBool32 odin_vulkan_debug_utils_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

void odin_vulkan_initialize(odin_render_device *render_device, odin_initialize_info* initialize_info)
{

    /* Create the render device. */
    *render_device = malloc(sizeof(odin_vulkan_render_device_t));
    aero_memset(*render_device, sizeof(odin_vulkan_render_device_t), 0);

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)*render_device;
    
    aero_memcpy(&vulkan_render_device->initialize_info, sizeof(odin_initialize_info), initialize_info, sizeof(odin_initialize_info));

    VkApplicationInfo application_info = { 0 };
    application_info.sType                  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pNext                  = NULL;
    application_info.pApplicationName       = initialize_info->application_name;
    application_info.applicationVersion     = VK_MAKE_VERSION(initialize_info->application_version.major, initialize_info->application_version.minor, initialize_info->application_version.patch);
    application_info.pEngineName            = initialize_info->engine_name;
    application_info.engineVersion          = VK_MAKE_VERSION(initialize_info->engine_version.major, initialize_info->engine_version.minor, initialize_info->engine_version.patch);
    application_info.apiVersion             = VK_API_VERSION_1_2;


    /* Get the validation layers. */
    uint32_t validation_layers_count = 0;
    const char **validation_layers = odin_vulkan_platform_get_validation_layers(&validation_layers_count);

    /* Double check this layers exist. */
    uint32_t validation_layers_enum_count = 0;
    vkEnumerateInstanceLayerProperties(&validation_layers_enum_count, NULL);

    VkLayerProperties* layer_properties = malloc(sizeof(VkLayerProperties) * validation_layers_enum_count);
    aero_memset(layer_properties, sizeof(VkLayerProperties) * validation_layers_enum_count, 0);

    vkEnumerateInstanceLayerProperties(&validation_layers_enum_count, layer_properties);


    int32_t validation_layers_found_count = 0;
    for(uint32_t i = 0; i < validation_layers_enum_count; i++)
    {
        for(uint32_t j = 0; j < validation_layers_count; j++)
        {

            const char* layer_name = layer_properties[i].layerName;

            bool indicator = false;
            aero_strcmp(layer_name, 256, validation_layers[j], &indicator);
            
            if(!indicator)
            {
                validation_layers_found_count++;
            }

        }
    }

    /* Somethings wrong with finding validation layers! */
    if(validation_layers_found_count != validation_layers_count)
    {
        ODIN_ERROR("o_vulkan_initialize.c", "Could not find specified Vulkan validation layers!");
    }

    /* Get the instance extensions. */
    uint32_t instance_extensions_count = 0;
    const char **instance_extensions = odin_vulkan_platform_get_instance_extensions(&instance_extensions_count);


    VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info = { 0 };
    debug_utils_messenger_create_info.sType             = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_utils_messenger_create_info.pNext             = NULL;
    debug_utils_messenger_create_info.flags             = 0;
    debug_utils_messenger_create_info.messageSeverity   = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |  VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_utils_messenger_create_info.messageType       = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_utils_messenger_create_info.pfnUserCallback   = odin_vulkan_debug_utils_callback;
    debug_utils_messenger_create_info.pUserData         = NULL;


    /* Create the vulkan instance. */
    VkInstanceCreateInfo instance_create_info = { 0 };
    instance_create_info.sType                      = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext                      = &debug_utils_messenger_create_info;
    instance_create_info.flags                      = 0;
    instance_create_info.pApplicationInfo           = &application_info;
    instance_create_info.enabledLayerCount          = validation_layers_count;
    instance_create_info.ppEnabledLayerNames        = validation_layers;
    instance_create_info.enabledExtensionCount          = instance_extensions_count;
    instance_create_info.ppEnabledExtensionNames    = instance_extensions;

    if(vkCreateInstance(&instance_create_info, NULL, &vulkan_render_device->instance) != VK_SUCCESS)
    {
        ODIN_ERROR("o_vulkan_initialize.c", "Could not create the Vulkan instance.");
    }

    /* Initialize the platform. */
    odin_vulkan_platform_initialize(vulkan_render_device);

}

void odin_vulkan_terminate(odin_render_device render_device)
{

    /* Get the vulkan render device. */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;


    /* Terminate the platform. */
    odin_vulkan_platform_terminate(vulkan_render_device);


    /* Destroy the Command Pool. */
    vkDestroyCommandPool(vulkan_render_device->device, vulkan_render_device->command_pool, NULL);

    /* Destroy the Memory Allocator. */
    vmaDestroyAllocator(vulkan_render_device->memory_allocator);

    /* Destroy the device. */
    vkDestroyDevice(vulkan_render_device->device, NULL);

    /* Destroy the vulkan instance. */
    vkDestroyInstance(vulkan_render_device->instance, NULL);


    /* Clear all render device memory. */
    free(render_device);
    render_device = NULL;
}

VkBool32 odin_vulkan_debug_utils_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    switch(messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            break;
    
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            ODIN_ERROR("o_vulkan_initialize.c", pCallbackData->pMessage);
            break;
    
    };


    return false;
}
