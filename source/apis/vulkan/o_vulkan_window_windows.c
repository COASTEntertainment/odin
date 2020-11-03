#include "odin/apis/vulkan/o_vulkan_window.h"

#include <Windows.h>

#include "odin/apis/vulkan/o_vulkan_render_device.h"


static int temp_monitors_iterator = 0;

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{

    int temp_monitors_count = GetSystemMetrics(SM_CMONITORS);

    if(temp_monitors_iterator > temp_monitors_count)
        temp_monitors_count = 0;
    
    odin_monitor_t* monitors = (odin_monitor_t *)dwData;

    monitors[temp_monitors_iterator].width = lprcMonitor->right - lprcMonitor->left;
    monitors[temp_monitors_iterator].height = lprcMonitor->bottom - lprcMonitor->top;

    temp_monitors_iterator++;

    return true;
}

/** \brief Gets a systems monitors and their information. */
void odin_vulkan_get_monitors(odin_render_device render_device, int *monitors_count, odin_monitor_t *monitors)
{

    /* First we need to get the display count. */
    int temp_monitors_count = GetSystemMetrics(SM_CMONITORS);
    *monitors_count = temp_monitors_count;

    if(monitors == NULL)
        return;

    if(monitors)
        EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)monitors);
        
}

void odin_vulkan_window_create(odin_render_device render_device, odin_window* window, const char* title, int x, int y, int width, int height, odin_window_style style, bool fullscreen, odin_monitor monitor)
{

    /* Get the vulkan render device */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;


    /* Create the windows window */
    DWORD window_style = 0;
    switch (style)
    {
    case odin_window_style_defalt:
        window_style = WS_OVERLAPPEDWINDOW;
        break;
    
    case odin_window_style_minimal:
        window_style = WS_BORDER;
        break;

    default:
        window_style = WS_OVERLAPPEDWINDOW;
        break;
    }

    CreateWindow("ODIN_VULKAN_WINDOW_CLASS", vulkan_render_device->initialize_info.application_name, window_style, x, y, width, height, 0, NULL, GetModuleHandle(NULL), NULL);

    

}

void odin_vulkan_window_destroy(odin_render_device render_device, odin_window window)
{
    
}

