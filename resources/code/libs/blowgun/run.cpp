#include "run.h"
#include "platform.h"

//#include <android/log.h>
using namespace blowgun;

void
blowgun::Run(
    CreateEnvironmentFunc create_environment_func,
    MainLoopFunc          main_loop_func,
    boost::any            platform_parameter)
{
    // Build the platform representation
    std::unique_ptr<Platform> platform(
        new Platform(create_environment_func, platform_parameter));

    // Initialize it
    platform->Initialize();

    // Let user's application play to its heart content
    main_loop_func(platform.get());

    // When the application is done, let's clean up the mess
    platform->Shutdown();
}
