#include "native_interface.h"

#ifdef WIN32

#include <type_traits>

#include <Windows.h>

/*
 * The sole purpose of this variable is to give sign to another
 * part of the application that the user closes the window. The
 * task will be easier if only Microsoft didn't design its Win32
 * GUI Programming the way they did.
 *
 * So, to work around this issue, I need to use "hidden" global
 * variable. For more information you can go to
 * http://www.autoitscript.com/forum/topic/66508-peekmessage-and-wm-close
 *
 * And yes, this is bad practice, I know.. I know.. But I have to
 * use it until I can find better (while still simple) solution.
 */
bool native_interface__win32___exit_is_requested = false;

namespace
{
	LRESULT CALLBACK WndProc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
	{
		LRESULT result = 0;

		switch (message)
		{
		case WM_CLOSE:
			native_interface__win32___exit_is_requested = true;
			PostQuitMessage(0);
			break;

		default:
			result = DefWindowProc(h_wnd, message, w_param, l_param);
			break;
		}

		return result;
	}
}

using namespace blowgun;

std::unique_ptr<const NativeInterface>
blowgun::CreateNativeInterface()
{
	HWND h_wnd = 0;

	WNDCLASS s_WC;
	s_WC.style = CS_HREDRAW | CS_VREDRAW;
	s_WC.cbClsExtra = 0;
	s_WC.cbWndExtra = 0;
	s_WC.lpfnWndProc = WndProc;
	s_WC.hInstance = 0;
	s_WC.hIcon = 0;
	s_WC.hCursor = LoadCursor(NULL, IDC_ARROW);
	s_WC.lpszMenuName = 0;
	s_WC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	s_WC.lpszClassName = "Test";
	unsigned int n_width = 800;
	unsigned int n_height = 600;

	ATOM register_class = RegisterClass(&s_WC);
	if (!register_class)
	{
		MessageBox(0, "Failed to register the window class", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	RECT s_rect;
	SetRect(&s_rect, 0, 0, n_width, n_height);
	AdjustWindowRectEx(&s_rect, WS_CAPTION | WS_SYSMENU, false, 0);
	h_wnd = CreateWindow("Test", "Test", WS_VISIBLE | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, n_width, n_height, NULL, NULL, 0, NULL);

	HDC h_DC = GetDC(h_wnd);
	if (!h_DC)
	{
		MessageBox(0, "Failed to create the device context", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	auto native_interface = new NativeInterface;
	native_interface->window  = h_wnd;
	native_interface->display = h_DC;
	native_interface->pixmap  = nullptr;

	return std::unique_ptr<const NativeInterface>(native_interface);
}

#endif // WIN32
