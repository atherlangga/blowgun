#include "platform.h"

#if defined(__linux__) && !defined(__ANDROID__)

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>

#include "types.h"

using namespace blowgun;

/*
 * PIMPL forward declaration
 */
class Platform::Impl
{
public:
    Impl(std::unique_ptr<const Environment> environment);

    void OnPreFrame_X11();
    void OnPostFrame_X11();
    void Initialize_X11();
    void Shutdown_X11();
    bool IsExitRequested_X11() const;

private:
    std::unique_ptr<const Environment> environment_;

private:
    ///
    // Disallow copy and assign.
    ///
    Impl(const Impl&); // = delete;
    Impl& operator=(const Impl&); // = delete;
};

////////////////////////////////////////////////////////////////////////////////

Platform::Platform(CreateEnvironmentFunc create_environment_func, void* param)
: impl_(new Impl(create_environment_func()))
{
}

Platform::~Platform()
{
}

void
Platform::Initialize()
{
    impl_->Initialize_X11();
}

void
Platform::Shutdown()
{
    impl_->Shutdown_X11();
}

void
Platform::OnPreFrame()
{
    impl_->OnPreFrame_X11();
}

void
Platform::OnPostFrame()
{
    impl_->OnPostFrame_X11();
}

bool
Platform::IsExitRequested() const
{
    return impl_->IsExitRequested_X11();
}


////////////////////////////////////////////////////////////////////////////////

#ifndef timeradd
# define timeradd(a, b, result)                             \
    do {                                                    \
        (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;       \
        (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;    \
        if ((result)->tv_usec >= 1000000)                   \
        {                                                   \
            ++(result)->tv_sec;                             \
            (result)->tv_usec -= 1000000;                   \
        }                                                   \
    } while (0)
#endif

#ifndef timersub
# define timersub(a, b, result)                             \
    do {                                                    \
        (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;       \
        (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;    \
        if ((result)->tv_usec < 0)                          \
        {                                                   \
            --(result)->tv_sec;                             \
            (result)->tv_usec += 1000000;                   \
        }                                                   \
    } while (0)
#endif

// This unit variable is used to flag when X11 receive
// message that user wants to quit
static bool native_interface__x11___exit_is_requested = false;

Platform::Impl::Impl(std::unique_ptr<const Environment> environment)
: environment_(std::move(environment))
{
}

void Platform::Impl::OnPreFrame_X11()
{
    XEvent event;

    struct timeval tv;
    struct timeval stv;
    struct timeval tv_period;

    Display* display = environment_->native_interface->display;
    Window   window  = environment_->native_interface->window;

    // Get reference for "WM_DELETE_WINDOW" message
    Atom delete_window_message = XInternAtom(
        display, "WM_DELETE_WINDOW", false
        );
    XSetWMProtocols(display, window, &delete_window_message, 1);

    // Get the File Descriptor for X11 Display
    fd_set in_fds;
    int x11_fd = ConnectionNumber(display);

    // Set Timer Period (this is how you configure the frequency):
    tv_period.tv_sec = 0;
    tv_period.tv_usec = 500000; // 500000us = 500ms = 0.5s

    // Set tv=1 sec so select() will timeout.
    tv.tv_sec = tv_period.tv_sec;
    tv.tv_usec = tv_period.tv_usec;

    gettimeofday(&stv, NULL);
    timeradd(&stv, &tv_period, &stv);

    // Set the file descriptor
    FD_ZERO(&in_fds);
    FD_SET(x11_fd, &in_fds);

    //
    // Wait for X Event *or* a Timer.
    //
    if (select(x11_fd + 1, &in_fds, NULL, NULL, &tv))
    {
        //
        // In case of there is X Event.
        //
        gettimeofday(&tv, NULL);
        timersub(&stv, &tv, &tv);  // set tv = remaining time.
    }
    else
    {
        //
        // In case that there's no X Event.
        //

        // Initialize timer variables again.
        // Set tv=1 sec so select() will timeout.
        tv.tv_sec = tv_period.tv_sec;
        tv.tv_usec = tv_period.tv_usec;

        gettimeofday(&stv, NULL);
        timeradd(&stv, &tv_period, &stv);
    }

    while (XPending(display))
    {
        XNextEvent(display, &event);
    }

    switch (event.type)
    {
    case ClientMessage:
        {
            u32 message = (u32) event.xclient.data.l[0];
            if (message == delete_window_message)
            {
                native_interface__x11___exit_is_requested = true;
            }
        }
        break;

    default:
        break;
    }
}

void Platform::Impl::OnPostFrame_X11()
{
    eglSwapBuffers(environment_->egl_display, environment_->egl_surface);
}

void Platform::Impl::Initialize_X11()
{
    eglMakeCurrent(
        environment_->egl_display,
        environment_->egl_surface,
        environment_->egl_surface,
        environment_->egl_context);
}

void Platform::Impl::Shutdown_X11()
{
    eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(environment_->egl_display, environment_->egl_context);
    eglDestroySurface(environment_->egl_display, environment_->egl_surface);
    eglTerminate(environment_->egl_display);
}

bool Platform::Impl::IsExitRequested_X11() const
{
    return native_interface__x11___exit_is_requested;
}

#endif // defined(__linux__) && !defined(__ANDROID__)
