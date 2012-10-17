#ifndef BLOWGUN_ENVIRONMENT_H
#define BLOWGUN_ENVIRONMENT_H

#include <memory>
#include <map>
#include <functional>

#include <EGL/egl.h>

#include "native_interface.h"

namespace blowgun
{

/**
 * The representation of underlying EGL environment
 * that will run the Application.
 */
struct Environment
{
    EGLDisplay      egl_display;
    EGLSurface      egl_surface;
    EGLContext      egl_context;
    std::unique_ptr<const NativeInterface> native_interface;
};

/**
 * Factory method to take care the instantiation of
 * `blowgun::Environment`.
 */
std::unique_ptr<const Environment> CreateEnvironment();

/**
 * Wrapper for CreateEnvironment.
 */
typedef std::function
    <std::unique_ptr<const Environment>
    (void)>
    CreateEnvironmentFunc;

} // namespace blowgun

#endif // BLOWGUN_ENVIRONMENT_H
