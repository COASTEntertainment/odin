#include "odin/odin.h"

int main()
{


    odin_load_api(odin_api_vulkan);


    odin_render_device render_device;
    odin_initialize(&render_device);


    


    odin_terminate(render_device);
}
