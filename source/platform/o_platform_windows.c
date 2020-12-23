#include "odin/o_platform_internal.h"

#include "aero/a_memory.h"

#include "odin/o_log.h"
#include "odin/o_input.h"
#include "odin/o_render_device.h"

#include "../input/o_input_windows_internal.h"

#include <Windows.h>
#include <dwmapi.h>


typedef struct odin_native_window_windows
{
	
	HWND handle;

} odin_native_window_windows_t, *odin_native_window_windows;


void odin_platform_init()
{

	WNDCLASSEX window_class 		= { 0 };
	window_class.cbSize 			= sizeof(WNDCLASSEX);
	window_class.style 				= CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc 		= window_proc;
	window_class.cbClsExtra 		= 0;
	window_class.cbWndExtra 		= 0;
	window_class.hInstance 			= NULL;
	window_class.hIcon 				= LoadIcon(NULL, IDI_WINLOGO);
	window_class.hCursor 			= LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground 		= NULL;
	window_class.lpszMenuName 		= NULL;
	window_class.lpszClassName 		= L"WINDOW_PLATFORM_CLASS";
	window_class.hIconSm 			= NULL;

	if(!RegisterClassEx(&window_class))
	{
		int er = GetLastError();
		ODIN_ERROR("o_platform_windows.c", "Could not create the window class.");
	}

	GlobalAddAtom("ODIN_WINDOW");
	GlobalAddAtom("ODIN_INPUT");

}

void odin_platform_create_native_window(
	odin_native_window*		native_window,
	odin_window		 		window,
	odin_input_device		input_device,
	const char*				title, 
    int						width, 
    int						height)
{

	HWND handle = CreateWindowEx(
		0,
		L"WINDOW_PLATFORM_CLASS",
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if(handle == NULL)
	{
		int er = GetLastError();
		ODIN_ERROR("o_platform_windows.c", "Could not create the windows window!");
	}

	ShowWindow(handle, SW_SHOW);

	/* Create the odin window. */
	odin_native_window_windows native_window_windows = AERO_NEW(odin_native_window_windows_t, 1);
	aero_memset(native_window_windows, sizeof(odin_native_window_windows_t), 0);

	native_window_windows->handle = handle;

	SetProp(handle, "ODIN_WINDOW", window);
	SetProp(handle, "ODIN_INPUT", input_device);


	*native_window = (odin_native_window)native_window_windows;

}

void odin_platform_destroy_native_window(
	odin_native_window native_window)
{

	odin_native_window_windows native_window_windows = (odin_native_window_windows)native_window;

	RemoveProp(native_window_windows->handle, "ODIN");
	RemoveProp(native_window_windows->handle, "ODIN_INPUT");

	DestroyWindow(native_window_windows->handle);

}

void odin_platform_get_native_window_size(
	odin_native_window          native_window,
	int*						width,
	int*						height)
{

	odin_native_window_windows native_window_windows = (odin_native_window_windows)native_window;

	RECT frame;
	DwmGetWindowAttribute(native_window_windows->handle, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));

	//rect should be `0, 0, 1280, 1024`
	//frame should be `7, 0, 1273, 1017`

	*width = (int)frame.right - frame.left;
	*height = (int)frame.bottom - frame.top;

}

#ifdef ODIN_HAS_VULKAN
void odin_platform_create_vulkan_surface(
	odin_native_window      native_window,
	VkInstance              instance,
	VkSurfaceKHR			*surface)
{
	AERO_NULL_CHECK(native_window);
	AERO_NULL_CHECK(instance);


	odin_native_window_windows native_window_windows = (odin_native_window_windows)native_window;

	VkWin32SurfaceCreateInfoKHR surface_create_info = { 0 };
	surface_create_info.flags = 0;
	surface_create_info.pNext = NULL;
	surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_create_info.hinstance = GetModuleHandle(NULL);
	surface_create_info.hwnd = native_window_windows->handle;

	if (vkCreateWin32SurfaceKHR(instance, &surface_create_info, NULL, surface) != VK_SUCCESS)
	{
		ODIN_ERROR("o_platform_windows.c", "Could not create the vulkan window surface!");
	}

}
#endif // ODIN_HAS_VULKAN