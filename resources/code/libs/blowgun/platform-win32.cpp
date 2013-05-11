#include "platform.h"

#ifdef WIN32

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <EGL/egl.h>

#include <boost/any.hpp>

#include <Windows.h>

#include "types.h"
#include "native_interface.h"
#include "environment.h"

using namespace blowgun;

/*
 * PIMPL forward declaration
 */
class Platform::Impl
{
public:
	Impl(std::unique_ptr<const Environment> environment);

	void OnPreFrame_Win32();
	void OnPostFrame_Win32();
	void Initialize_Win32();
	void Shutdown_Win32();
	bool IsExitRequested_Win32() const;

private:
	std::unique_ptr<const Environment> environment_;

private:
	/*
	 * Disallow copy and assign.
	 */
	Impl(const Impl&); // = delete;
	Impl& operator=(const Impl&); // = delete;
};

Platform::Platform(
	CreateEnvironmentFunc create_environment_func,
	boost::any            /*param*/)
: impl_(new Impl(create_environment_func()))
{
}

Platform::~Platform()
{
}

void
Platform::Initialize()
{
	impl_->Initialize_Win32();
}

void
Platform::Shutdown()
{
	impl_->Shutdown_Win32();
}

void
Platform::OnPreFrame()
{
	impl_->OnPreFrame_Win32();
}

void
Platform::OnPostFrame()
{
	impl_->OnPostFrame_Win32();
}

bool
Platform::IsExitRequested() const
{
	return impl_->IsExitRequested_Win32();
}


Platform::Impl::Impl(
	std::unique_ptr<const Environment> environment
	)
: environment_(std::move(environment))
{
}

void Platform::Impl::OnPreFrame_Win32()
{
	MSG msg;
	bool message_is_coming = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);

	if (message_is_coming)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Platform::Impl::OnPostFrame_Win32()
{
	eglSwapBuffers(environment_->egl_display, environment_->egl_surface);
}

void Platform::Impl::Initialize_Win32()
{
	eglMakeCurrent(
		environment_->egl_display,
		environment_->egl_surface,
		environment_->egl_surface,
		environment_->egl_context
		);
}

void Platform::Impl::Shutdown_Win32()
{
	eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(environment_->egl_display, environment_->egl_context);
	eglDestroySurface(environment_->egl_display, environment_->egl_surface);
	eglTerminate(environment_->egl_display);
}

bool Platform::Impl::IsExitRequested_Win32() const
{
	extern bool native_interface__win32___exit_is_requested;
	return native_interface__win32___exit_is_requested;
}

#endif // WIN32
