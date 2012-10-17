#include "platform.h"

#if defined(__ANDROID__)

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <jni.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <android/log.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#include <EGL/egl.h>

#include "types.h"
#include "environment.h"

using namespace blowgun;

/*
 * PIMPL forward declaration
 */
class Platform::Impl
{
public:
    Impl(std::unique_ptr<const Environment> environment, void* param);

    void OnPreFrame_Android();
    void OnPostFrame_Android();
    void Initialize_Android();
    void Shutdown_Android();
    bool IsExitRequested_Android() const;

private:
    std::unique_ptr<const Environment> environment_;
    ANativeActivity* activity_;

private:
    ///
    // Disallow copy and assign.
    ///
    Impl(const Impl&); // = delete;
    Impl& operator=(const Impl&); // = delete;
};

////////////////////////////////////////////////////////////////////////////////

Platform::Platform(CreateEnvironmentFunc create_environment_func, void* param)
: impl_(new Impl(create_environment_func(), param))
{
}

Platform::~Platform()
{
}

void
Platform::Initialize()
{
    impl_->Initialize_Android();
}

void
Platform::Shutdown()
{
    impl_->Shutdown_Android();
}

void
Platform::OnPreFrame()
{
    impl_->OnPreFrame_Android();
}

void
Platform::OnPostFrame()
{
    impl_->OnPostFrame_Android();
}

bool
Platform::IsExitRequested() const
{
    return impl_->IsExitRequested_Android();
}


////////////////////////////////////////////////////////////////////////////////

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Training", __VA_ARGS__))

static void onDestroy(ANativeActivity* activity)
{
    LOGI("Destroy: %p\n", (void*)activity);
}

static void onStart(ANativeActivity* activity)
{
    LOGI("Start: %p\n", (void*)activity);
}

static void onResume(ANativeActivity* activity)
{
    LOGI("Resume: %p\n", (void*)activity);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    LOGI("SaveInstanceState: %p\n", (void*)activity);

    return NULL;
}

static void onPause(ANativeActivity* activity)
{
    LOGI("Pause: %p\n", (void*)activity);
}

static void onStop(ANativeActivity* activity)
{
    LOGI("Stop: %p\n", (void*)activity);
}

static void onConfigurationChanged(ANativeActivity* activity)
{
    LOGI("ConfigurationChanged: %p\n", (void*)activity);
}

static void onLowMemory(ANativeActivity* activity)
{
    LOGI("LowMemory: %p\n", (void*)activity);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
    LOGI("WindowFocusChanged: %p -- %d\n", (void*)activity, focused);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    LOGI("NativeWindowCreated: %p -- %p\n", (void*)activity, (void*)window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    LOGI("NativeWindowDestroyed: %p -- %p\n", (void*)activity, (void*)window);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    LOGI("InputQueueCreated: %p -- %p\n", (void*)activity, (void*)queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    LOGI("InputQueueDestroyed: %p -- %p\n", (void*)activity, (void*)queue);
}

Platform::Impl::Impl (
    std::unique_ptr<const Environment> environment,
    void* param)
: environment_(std::move(environment))
{
    activity_ = (ANativeActivity*) param;
}

void Platform::Impl::OnPreFrame_Android()
{
    // TODO: Implementation for Android
}

void Platform::Impl::OnPostFrame_Android()
{
    eglSwapBuffers(environment_->egl_display, environment_->egl_surface);
}

void Platform::Impl::Initialize_Android()
{
    activity_->callbacks->onDestroy = onDestroy;
    activity_->callbacks->onStart = onStart;
    activity_->callbacks->onResume = onResume;
    activity_->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity_->callbacks->onPause = onPause;
    activity_->callbacks->onStop = onStop;
    activity_->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity_->callbacks->onLowMemory = onLowMemory;
    activity_->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity_->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity_->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity_->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity_->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

    activity_->instance = NULL;

    eglMakeCurrent(
        environment_->egl_display,
        environment_->egl_surface,
        environment_->egl_surface,
        environment_->egl_context);
}

void Platform::Impl::Shutdown_Android()
{
    eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(environment_->egl_display, environment_->egl_context);
    eglDestroySurface(environment_->egl_display, environment_->egl_surface);
    eglTerminate(environment_->egl_display);
}

bool Platform::Impl::IsExitRequested_Android() const
{
    // TODO: Real implementation for Android
    return false;
}

#endif // defined(__ANDROID__)
