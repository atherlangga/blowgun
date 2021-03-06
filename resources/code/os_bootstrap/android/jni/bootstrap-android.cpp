#include <memory>
#include <functional>

#include <android/native_activity.h>

#include <blowgun/environment-default.h>
#include <blowgun/platform.h>
#include <blowgun/run.h>

#include "../../../application/core/04-CameraMovement/camera_movement_application.h"

namespace
{
    void main_loop(blowgun::Platform* platform)
    {
        CameraMovementApplication app;
        app.OnInitialization();

        while (!platform->IsExitRequested())
        {
            platform->OnPreFrame();

            app.OnUpdate();
            app.OnDraw();

            platform->OnPostFrame();
        }

        app.OnDestroy();
    }
}

void ANativeActivity_onCreate(ANativeActivity* activity,
        void* savedState, size_t savedStateSize)
{
    auto create_environment_func = []() -> std::unique_ptr<blowgun::Environment>
    {
        auto create_native_interface_func = blowgun::CreateNativeInterface;

        auto create_config_attribs_func = []() -> blowgun::ConfigAttributes
        {
            blowgun::ConfigAttributes config_attribs_map;
            config_attribs_map.insert(
                std::make_pair(EGL_SURFACE_TYPE, EGL_WINDOW_BIT));
            config_attribs_map.insert(
                std::make_pair(EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT));
            return config_attribs_map;
        };

        auto create_context_attribs_func = []() -> blowgun::ContextAttributes
        {
            blowgun::ContextAttributes context_attribs_map;
            context_attribs_map.insert(
                std::make_pair(EGL_CONTEXT_CLIENT_VERSION, 2));
            return context_attribs_map;
        };

        return blowgun::CreateEnvironmentDefault(
            create_native_interface_func,
            create_config_attribs_func,
            create_context_attribs_func);
    };

    blowgun::Run(create_environment_func, main_loop, activity);
}
