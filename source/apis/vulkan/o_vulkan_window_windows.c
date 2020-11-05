#include "odin/apis/vulkan/o_vulkan_window.h"

#include <Windows.h>

#include "aero/a_memory.h"

#include "odin/apis/vulkan/o_vulkan_render_device.h"


static int temp_monitors_iterator = 0;

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{

    int temp_monitors_count = GetSystemMetrics(SM_CMONITORS);

    if(temp_monitors_iterator > temp_monitors_count)
        temp_monitors_count = 0;
    
    odin_monitor* monitors = (odin_monitor *)dwData;

    monitors[temp_monitors_iterator] = malloc(sizeof(odin_monitor_t));

    monitors[temp_monitors_iterator]->width = lprcMonitor->right - lprcMonitor->left;
    monitors[temp_monitors_iterator]->height = lprcMonitor->bottom - lprcMonitor->top;

    temp_monitors_iterator++;

    return true;
}

/** \brief Gets a systems monitors and their information. */
void odin_vulkan_get_monitors(odin_render_device render_device, int *monitors_count, odin_monitor *monitors)
{

    /* First we need to get the display count. */
    int temp_monitors_count = GetSystemMetrics(SM_CMONITORS);
    *monitors_count = temp_monitors_count;

    if(monitors == NULL)
        return;

    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)monitors);        
}

typedef struct odin_windows_window_t
{

    HWND handle;

} odin_windows_window_t;

void odin_vulkan_window_create(odin_render_device render_device, odin_window* window, const char* title, int x, int y, int width, int height, odin_window_style style, bool fullscreen, odin_monitor monitor)
{

    /* Get the vulkan render device */
    odin_vulkan_render_device vulkan_render_device = (odin_vulkan_render_device)render_device;


    /* Create the windows window */
    DWORD window_style = 0;
    switch (style)
    {
    case odin_window_style_defalt:
        window_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        break;
    
    case odin_window_style_minimal:
        window_style = 0;
        break;

    default:
        window_style = WS_OVERLAPPEDWINDOW;
        break;
    }

    /* Create the windows window. */
    odin_windows_window_t* vulkan_window = malloc(sizeof(odin_windows_window_t));
    aero_memset(vulkan_window, sizeof(odin_windows_window_t), 0);

    *window = (odin_window)vulkan_window;

    HINSTANCE hInstance = GetModuleHandle(NULL);
    HWND handle = CreateWindowEx(
        0,                              // Optional window styles.
        "ODIN_VULKAN_WINDOW_CLASS",                     // Window class
        title,    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );


    /* Make sure the style is correct. */
    switch (style)
    {
    
    case odin_window_style_minimal:
        SetWindowLong(handle, GWL_STYLE, 0); //remove all window styles, check MSDN for details
        break;

    default:
        break;
    }


    ShowWindow(handle, SW_SHOW);

}

void odin_vulkan_window_destroy(odin_render_device render_device, odin_window window)
{

    /* Get the vulkan window. */
    odin_windows_window_t* vulkan_window = (odin_windows_window_t*)window;

    DestroyWindow(vulkan_window->handle);

}

