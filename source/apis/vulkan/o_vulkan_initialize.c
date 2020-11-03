#include "odin/apis/vulkan/o_vulkan_initialize.h"

#include <stdlib.h>

#include <vulkan/vulkan.h>

#include "odin/apis/vulkan/o_vulkan_render_device.h"
#include "odin/apis/vulkan/o_vulkan_platform.h"


void odin_vulkan_initialize(odin_render_device *render_device, odin_initialize_info* initialize_info)
{

    /* Create the render device */
    *render_device = malloc(sizeof(odin_vulkan_render_device_t));

    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)*render_device;

    


    VkApplicationInfo application_info = { 0 };
    application_info.sType                  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pNext                  = NULL;
    application_info.pApplicationName       = initialize_info->application_name;
    application_info.applicationVersion     = VK_MAKE_VERSION(initialize_info->application_version.major, initialize_info->application_version.minor, initialize_info->application_version.patch);
    application_info.pEngineName            = initialize_info->engine_name;
    application_info.engineVersion          = VK_MAKE_VERSION(initialize_info->engine_version.major, initialize_info->engine_version.minor, initialize_info->engine_version.patch);
    application_info.apiVersion             = VK_API_VERSION_1_2;


    /* Get the validation layers */
    uint32_t validation_layers_count = 0;
    const char **validation_layers = odin_vulkan_platform_get_validation_layers(&validation_layers_count);

    /* Get the instance extensions */
    uint32_t instance_extensions_count = 0;
    const char **instance_extensions = odin_vulkan_platform_get_instance_extensions(&instance_extensions_count);


    /* Create the vulkan instance */
    VkInstanceCreateInfo instance_create_info = { 0 };
    instance_create_info.sType                      = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext                      = NULL;
    instance_create_info.flags                      = 0;
    instance_create_info.pApplicationInfo           = &application_info;
    instance_create_info.enabledLayerCount          = validation_layers_count;
    instance_create_info.ppEnabledLayerNames        = validation_layers;
    instance_create_info.enabledLayerCount          = instance_extensions_count;
    instance_create_info.ppEnabledExtensionNames    = instance_extensions;

    vkCreateInstance(&instance_create_info, NULL, &vulkan_render_device->instance);

    /* Initialize the platform */
    odin_vulkan_platform_initialize(vulkan_render_device);

}

void odin_vulkan_terminate(odin_render_device render_device)
{

    /* Get the vulkan render device */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;

    /* Terminate the platform */
    odin_vulkan_platform_terminate(vulkan_render_device);

    /* Destroy the vulkan instance */
    vkDestroyInstance(vulkan_render_device->instance, NULL);

}