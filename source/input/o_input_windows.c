#include "odin/o_input.h"
#include "odin/o_log.h"

#include "aero/a_memory.h"
#include "aero/a_thread_pool.h"

#include "Windows.h"

#include "odin/o_window.h"



typedef struct odin_input_device_windows
{

    odin_input_window_events_processor      window_processor;
    odin_input_keyboard_events_processor    keyboard_processor;
    odin_input_mouse_events_processor       mouse_processor;
} odin_input_device_windows_t, *odin_input_device_windows;


void odin_input_device_create(odin_input_device* input_device)
{

    *input_device = malloc(sizeof(odin_input_device_windows));
    odin_input_device_windows windows_input_device = (odin_input_device_windows)input_device;

    /* Register the raw input devices. */
    RAWINPUTDEVICE raw_input_devices[2];
    raw_input_devices[0].usUsagePage = 0x01;
    raw_input_devices[0].usUsage = 0x02;
    raw_input_devices[0].dwFlags = RIDEV_NOLEGACY;
    raw_input_devices[0].hwndTarget = 0;

    raw_input_devices[1].usUsagePage = 0x01;
    raw_input_devices[1].usUsage = 0x06;
    raw_input_devices[1].dwFlags = RIDEV_NOLEGACY;
    raw_input_devices[1].hwndTarget = 0;

    if(RegisterRawInputDevices(raw_input_devices, 2, sizeof(raw_input_devices[0])) == FALSE)
    {
        ODIN_ERROR("o_input_windows.c", "Could not create a raw input device!");
    }
}


void odin_input_device_destroy(odin_input_device input_device)
{

    odin_input_device_windows input_device_windows = (odin_input_device_windows)input_device;

    free(input_device_windows);
    input_device = NULL;
}


void odin_input_device_set_window_processor (odin_input_device input_device, odin_input_window_events_processor window_processor)
{

    odin_input_device_windows windows_input_device = (odin_input_device_windows)input_device;

    windows_input_device->window_processor = window_processor;
}


void odin_input_device_set_keyboard_processor(odin_input_device input_device, odin_input_keyboard_events_processor keyboard_processor)
{

    odin_input_device_windows windows_input_device = (odin_input_device_windows)input_device;

    windows_input_device->keyboard_processor = keyboard_processor;
}


void odin_input_device_set_mouse_processor(odin_input_device input_device, odin_input_mouse_events_processor mouse_processor)
{

    odin_input_device_windows windows_input_device = (odin_input_device_windows)input_device;

    windows_input_device->mouse_processor = mouse_processor;
}


void odin_input_poll()
{

    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    odin_window window = (odin_window)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    odin_input_device input_device;


    odin_event_window_data_t window_data = { 0 };


    odin_input_device_windows input_device_windows = NULL;

    if(window)
    {
        odin_window_get_input_device(window, &input_device);
        input_device_windows = (odin_input_device_windows)input_device;
    }
    else
    {


    }
    


    switch(uMsg)
    {



        /* Check all window cases. */
        case WM_CREATE:
            CREATESTRUCT *create_struct = (CREATESTRUCT*)lParam;
            window = (odin_window)create_struct->lpCreateParams;

            if(lParam)
            {

            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);

            odin_window_get_input_device(window, &input_device);
            input_device_windows = (odin_input_device_windows)input_device;
            }


            input_device_windows->window_processor(window, odin_event_window_open, window_data);
        return 0;



        case WM_CLOSE:
            input_device_windows->window_processor(window, odin_event_window_close, window_data);
        return 0;

        case WM_SIZE:
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);

            window_data.width = width;
            window_data.height = height;

            input_device_windows->window_processor(window, odin_event_window_close, window_data);
        return 0;

        case WM_SETFOCUS:

        return 0;

        case WM_PAINT:
            ValidateRect(hwnd, NULL);
        return 0;

        /* Check for all input cases. */
        case WM_INPUT:



            UINT size = 0;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
            
            LPBYTE byte_info = malloc(sizeof(BYTE) * size);

            if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, byte_info, &size, sizeof(RAWINPUTHEADER)) != size)
            {
                ODIN_ERROR("o_input_windows.c", "Raw input returns incorrect size!");
            }

            RAWINPUT* raw = (RAWINPUT*)byte_info;


            /* Check what type of input we have. */
            switch(raw->header.dwType)
            {

                case RIM_TYPEKEYBOARD:

                    raw->data.keyboard;

                return 0;

                case RIM_TYPEMOUSE:

                    raw->data.mouse;

                return 0;

            };

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);


    }

    return 0;
}