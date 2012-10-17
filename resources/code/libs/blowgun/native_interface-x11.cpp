#include "native_interface.h"

#if defined(__linux__) && !defined(__ANDROID__)

#include <iostream>
#include <memory>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>

using namespace blowgun;

std::unique_ptr<const NativeInterface>
blowgun::CreateNativeInterface()
{
    Window          x11_window   = 0;
    Display*        x11_display  = 0;
    long            x11_screen   = 0;
    XVisualInfo*    x11_visual   = 0;
    Colormap        x11_colormap = 0;

    int width = 800;
    int height = 600;

    Window                  root_window;
    XSetWindowAttributes    window_attribute;
    unsigned int            mask;
    int                     depth;

    x11_display = XOpenDisplay(0);
    if (!x11_display)
    {
        throw "Error: Unable to open display";
    }

    x11_screen = XDefaultScreen(x11_display);
    root_window = RootWindow(x11_display, x11_screen);
    depth = DefaultDepth(x11_display, x11_screen);
    x11_visual = new XVisualInfo;
    XMatchVisualInfo(x11_display, x11_screen, depth, TrueColor, x11_visual);

    if (!x11_visual)
    {
        throw "Error: Unable to acquire visual";
    }

    x11_colormap = XCreateColormap(
        x11_display, root_window, x11_visual->visual, AllocNone);

    window_attribute.colormap = x11_colormap;
    window_attribute.event_mask = StructureNotifyMask | ExposureMask |
        ButtonPressMask | ButtonReleaseMask |
        KeyPressMask | KeyReleaseMask;

    mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

    x11_window = XCreateWindow(
            x11_display,
            RootWindow(x11_display, x11_screen),
            0, 0, width, height,
            0,
            CopyFromParent,
            InputOutput,
            CopyFromParent,
            mask,
            &window_attribute);

    XMapWindow(x11_display, x11_window);

    delete x11_visual;

    auto native_interface = new NativeInterface;
    native_interface->window  = x11_window;
    native_interface->display = x11_display;
    native_interface->pixmap  = 0;

    return std::unique_ptr<const NativeInterface>(native_interface);
}

#endif // defined(__linux__) && !defined(__ANDROID__)
