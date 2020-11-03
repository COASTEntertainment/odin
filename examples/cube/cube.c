#include "odin/odin.h"

#include <stdlib.h>
#include <string.h>

int main()
{

    odin_load_api(odin_api_vulkan);

    odin_initialize_info initialize_info = { 0 };
    initialize_info.application_name = "Cube Example";
    initialize_info.application_version = (odin_version){ 0, 0, 1 };
    initialize_info.engine_name = "Example Engine";
    initialize_info.engine_version = (odin_version){ 0, 0, 1 };

    odin_render_device render_device;
    odin_initialize(&render_device, &initialize_info);

    /* Get the monitors count. */
    int monitors_count = 0;
    odin_get_monitors(render_device, &monitors_count, NULL);

    odin_monitor_t* monitors = malloc(sizeof(odin_monitor_t) * monitors_count);
    memset(monitors, 0, sizeof(odin_monitor_t) * monitors_count);

    odin_get_monitors(render_device, &monitors_count, monitors);

    int width = monitors[0].width;
    int height = monitors[0].height;


    odin_terminate(render_device);
}
