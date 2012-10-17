#include "environment-default.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#ifdef __ANDROID__
#include <android/native_window.h>
#include <android/log.h>

#define  LOG_TAG    "blowgun"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif // __ANDROID__

#include "types.h"
#include "native_interface.h"

using namespace blowgun;

std::unique_ptr<Environment>
blowgun::CreateEnvironmentDefault(
    CreateNativeInterfaceFunc   create_native_interface_func,
    CreateConfigAttributesFunc  create_config_attributes_func,
    CreateContextAttributesFunc create_context_attributes_func
)
{
    EGLDisplay  egl_display = 0;
    EGLConfig   egl_config  = 0;
    EGLSurface  egl_surface = 0;
    EGLContext  egl_context = 0;

    auto native_interface   = create_native_interface_func();
    auto config_attributes  = create_config_attributes_func();
    auto context_attributes = create_context_attributes_func();

#ifdef __ANDROID__
    auto egl_native_display = EGL_DEFAULT_DISPLAY;
#else
    auto egl_native_display = native_interface->display;
#endif

    //
    // The EGL standard expects configuration to be laid out in
    // sequential manner, something like this:
    //
    //     EGLint arr[..]
    //     arr[0] = EGL_SURFACE_TYPE;    arr[1] = EGL_WINDOW_BIT;
    //     arr[2] = EGL_RENDERABLE_TYPE; arr[3] = EGL_OPENGL_ES2_BIT;
    //     // etc
    //     arr[..] = EGL_NONE; // this is marker for the end
    //                         // of configuration array
    //
    // However, I think std::map is a better and more readable
    // representation. So, this step is to convert the mapping
    // of configuration attributes to sequential array.
    //
    std::vector<EGLint> config_attribs;
    std::for_each(config_attributes.begin(), config_attributes.end(),
        [&config_attribs](std::pair<EGLint, EGLint> i) {
            config_attribs.push_back(i.first);
            config_attribs.push_back(i.second);
    });
    config_attribs.push_back(EGL_NONE);

    //
    // The rule for configuration attributes mapping above also
    // applies for context attributes mapping.
    //
    std::vector<EGLint> context_attribs;
    std::for_each(
        context_attributes.begin(),
        context_attributes.end(),
        [&context_attribs](std::pair<EGLint, EGLint> i) {
            context_attribs.push_back(i.first);
            context_attribs.push_back(i.second);
    });
    context_attribs.push_back(EGL_NONE);

    //
    // Now that the preparation is done, let's get the real work done
    // (i.e. to get EGL Display, EGL Surface, and EGL Context values)
    //
    egl_display = eglGetDisplay(egl_native_display);
    if(egl_display == (EGLDisplay) EGL_NO_DISPLAY)
    {
        egl_display = eglGetDisplay(
            (EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
    }

    EGLint major_version, minor_version;
    if (!eglInitialize(egl_display, &major_version, &minor_version))
    {
        std::string error_message = "Error: eglInitialize failed.";
        std::cerr << error_message.c_str() << std::endl;
        throw new std::runtime_error(error_message);
    }

    int total_configs;
    if (!eglChooseConfig(egl_display, &config_attribs[0],
        &egl_config, 1, &total_configs)
        ||
        (total_configs > 1))
    {
        std::string error_message = "Error: eglChooseConfig failed.";
        std::cerr << error_message.c_str() << std::endl;
        throw new std::runtime_error(error_message);
    }

#ifdef __ANDROID__
    int format;
    eglGetConfigAttrib(egl_display, egl_config,
        EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(native_interface->window,
        0, 0, format);
    auto egl_native_window = native_interface->window;
#else
    auto egl_native_window = native_interface->window;
#endif

    egl_surface = eglCreateWindowSurface(egl_display, egl_config,
        egl_native_window, NULL);
    if (eglGetError() != EGL_SUCCESS)
    {
        std::string error_message =
            "Error: eglCreateWindowSurface failed.";
        std::cerr << error_message.c_str() << std::endl;
        throw new std::runtime_error(error_message);
    }

    egl_context = eglCreateContext(egl_display, egl_config,
        NULL, &context_attribs[0]);
    if (eglGetError() != EGL_SUCCESS)
    {
        std::string error_message = "Error: eglCreateContext failed.";
        std::cerr << error_message.c_str() << std::endl;
        throw new std::runtime_error(error_message);
    }

    auto environment = new Environment;
    environment->egl_display = egl_display;
    environment->egl_surface = egl_surface;
    environment->egl_context = egl_context;
    environment->native_interface = std::move(native_interface);

    return std::unique_ptr<Environment>(environment);
}
