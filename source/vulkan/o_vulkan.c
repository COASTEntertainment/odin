#include "o_vulkan.h"

#include "assert.h"

#include "o_vulkan_platform.h"


/* Creates a validation debug messenger */
VkResult odin_vulkan_create_debug_utils_messenger
(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* debugMessengerCreateInfo, const VkAllocationCallbacks* allocationCallbacks, VkDebugUtilsMessengerEXT* pMessenger);

/* Destroys a validation debug messenger */
void odin_vulkan_destroy_debug_utils_messenger
(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);

/* Debug callback for validation layers */
VKAPI_ATTR VkBool32 VKAPI_CALL odin_vulkan_validation_callback
(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

extern void odin_vulkan_get_api_capabilities                                    
(odin_api_capabilities* capabilities)
{
    capabilities->physical_devices = true;
}

extern void odin_vulkan_init                                                    
(odin_data** data, odin_initalization_information* initalization_info)
{

    /* Allocate the data pointer */
    odin_vulkan_data* vulkan_data = malloc(sizeof(odin_vulkan_data));
    memset(*data, sizeof(odin_vulkan_data), 0);

    /* Set the data pointer */
    *data = (odin_data*)vulkan_data;

    /* Check if we need any validation */
    #if !defined(NDEBUG)
        vulkan_data->validation_enabled = true;
    #else
        vulkan_data->validation_enabled = false;
    #endif

    /* Create Instance */
    VkApplicationInfo application_info = { 0 };
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pNext = NULL;
    application_info.pApplicationName = initalization_info->application_name;
    application_info.applicationVersion = (uint32_t)VK_MAKE_VERSION(initalization_info->application_version_major, initalization_info->application_version_minor, initalization_info->application_version_patch);
    application_info.pEngineName = initalization_info->engine_name;
    application_info.engineVersion = (uint32_t)VK_MAKE_VERSION(initalization_info->engine_version_major, initalization_info->engine_version_minor, initalization_info->engine_version_patch);
    application_info.apiVersion = VK_API_VERSION_1_2;

    /* Get platform validation layers */
    uint32_t validation_layers_count = 0;
    const char** validation_layers = odin_vulkan_platform_get_validation_layers(&validation_layers_count);

    /* Get platform instance extensions */
    uint32_t instance_extensions_count = 0;
    const char** instance_extensions = odin_vulkan_platform_get_instance_extensions(&instance_extensions_count);

    /* Build the debug messenger create info */
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = { 0 };
    debug_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_create_info.pNext = NULL;
    debug_messenger_create_info.flags = 0;
    debug_messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_create_info.pfnUserCallback = odin_vulkan_validation_callback;
    debug_messenger_create_info.pUserData = vulkan_data;

    /* Build the instance create info */
    VkInstanceCreateInfo instance_create_info = { 0 };
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext = vulkan_data->validation_enabled ? &debug_messenger_create_info : NULL;
    instance_create_info.flags = 0;
    instance_create_info.pApplicationInfo = &application_info;
    instance_create_info.enabledLayerCount = vulkan_data->validation_enabled ? validation_layers_count : 0;
    instance_create_info.ppEnabledLayerNames = vulkan_data->validation_enabled ? validation_layers : NULL,
    instance_create_info.enabledExtensionCount = instance_extensions_count;
    instance_create_info.ppEnabledExtensionNames = instance_extensions;

    /* Create the vulkan application instance */
    VK_CHECK(vkCreateInstance(&instance_create_info, NULL, &vulkan_data->instance), "o_vulkan.c", 0x1001, "Vulkan: Could not create instance!");

    /* Create the vulkan debug messenger */
    VK_CHECK(odin_vulkan_create_debug_utils_messenger(vulkan_data->instance, &debug_messenger_create_info, NULL, &vulkan_data->debug_utils_messenger), "o_vulkan.c", 0x1002, "Vulkan: Could not create debug messenger!");

    /* Init platform */
    odin_vulkan_platform_init((odin_data*)vulkan_data);


}

void odin_vulkan_free                                                    
(odin_data* data)
{
    /* Get the vulkan data */
    odin_vulkan_data* vulkan_data = (odin_vulkan_data*)data;



    /* Destroy the debug messenger */
    vkDestroyDebugUtilsMessengerEXT(vulkan_data->instance, vulkan_data->debug_utils_messenger, NULL);

    /* Destroy the instance */
    vkDestroyInstance(vulkan_data->instance, NULL);

}

VKAPI_ATTR VkBool32 VKAPI_CALL odin_vulkan_specific_validation_callback
(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{

    switch(messageSeverity)
    {

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            ODIN_ERROR("o_vulkan.c", __LINE__, 0x1003, pCallbackData->pMessage);
        break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            ODIN_ERROR("o_vulkan.c", __LINE__, 0x1003, pCallbackData->pMessage);
        break;

    };

    return false;
}

VkResult odin_vulkan_specific_create_debug_utils_messenger
(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{

    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(func)
        return func(instance, pCreateInfo, pAllocator, pMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    
}

void odin_vulkan_specific_destroy_debug_utils_messenger
(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{

    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func)
        func(instance, messenger, pAllocator);

}