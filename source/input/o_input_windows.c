#include "odin/o_input.h"
#include "odin/o_log.h"


typedef struct odin_input_device_windows
{

    odin_input_window_events_processor      window_processor;
    odin_input_keyboard_events_processor    keyboard_processor;
    odin_input_mouse_events_processor       mouse_processor;
    odin_input_gamepad_events_processor     gamepad_processor;

} *odin_input_device_windows;


void odin_input_device_create(odin_input_device* input_device)
{

}

void odin_input_device_destroy(odin_input_device input_device)
{

}

void odin_input_device_set_processor(odin_input_device input_device, odin_event_type type, odin_input_events_processor events_processor)
{

    odin_input_device_windows windows_input_device = (odin_input_device_windows)input_device;


    switch(type)
    {

        case odin_event_type_window:

            windows_input_device->window_processor = (odin_input_window_events_processor)events_processor;
            return;

        break;

        case odin_event_type_keyboard:

            windows_input_device->keyboard_processor = (odin_input_keyboard_events_processor)events_processor;
            return;

        break;

        case odin_event_type_mouse:

            windows_input_device->mouse_processor = (odin_input_mouse_events_processor)events_processor;
            return;

        break;

        case odin_event_type_gamepad:

            windows_input_device->gamepad_processor = (odin_input_gamepad_events_processor)events_processor;
            return;

        break;

        default:
            ODIN_ERROR("o_input_windows.c", "Events processor choice invalid type!");
            return;
    
    }

}