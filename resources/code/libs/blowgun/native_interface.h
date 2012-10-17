#ifndef BLOWGUN_NATIVE_INTERFACE_H
#define BLOWGUN_NATIVE_INTERFACE_H

#include <memory>
#include <functional>

#include <EGL/egl.h>

namespace blowgun
{

/**
 * NativeInterface is the interface that will be used by
 * OpenGL ES implementation to do all the operations.
 *
 * To put it in another term, NativeInterface is the canvas
 * where OpenGL ES implementation can draw upon.
 */
struct NativeInterface
{
    EGLNativeWindowType  window;
    EGLNativeDisplayType display;
    EGLNativePixmapType  pixmap;
};


/**
 * Create and prepare NativeInterface.
 *
 * Please note that of all NativeInterface members, only
 * `window` is mandatory. The others is not guaranteed
 * to be filled. The reason is that not all platforms
 * required the other members.
 */
std::unique_ptr<const NativeInterface> CreateNativeInterface();

/**
 * Wrapper for CreateNativeInterface.
 */
typedef std::function
    <std::unique_ptr<const NativeInterface>
    (void)>
    CreateNativeInterfaceFunc;

}

#endif // BLOWGUN_NATIVE_INTERFACE_H
