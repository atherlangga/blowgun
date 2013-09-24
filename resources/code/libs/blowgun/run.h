#ifndef BLOWGUN_RUN_H
#define BLOWGUN_RUN_H

#include <functional>
#include <memory>
#include <boost/any.hpp>

#include "native_interface.h"
#include "environment.h"

namespace blowgun
{

class Platform;

/**
 * The main loop.
 */
typedef std::function<void (Platform*)> MainLoopFunc;

/**
 * Runs the main loop.
 *
 * The key argument - main_loop_func - has some requirements:
 * - It has to call Platform::OnPreFrame() before frame processing.
 * - It has to call Platform::OnPostFrame() after frame processing.
 * - It has to check whether Platform::IsExitRequested() returns
 *   true.
 *
 * Here's an example for illustration purpose:
 *
 * <pre>
 * {@code
 *
 *     DoApplicationInitialization();
 *
 *     // Start the magic.
 *     while (!platform->IsExitRequested())
 *     {
 *         platform->OnPreFrame();
 *
 *         DoApplicationSpecificStuffs();
 *
 *         platform->OnPostFrame();
 *     }
 *
 * }
 * </pre>
 *
 * @param   create_environment_func
 *          User-defined function to create the Environment.
 * @param   main_loop_func
 *          The key argument. This is the user-defined main loop
 *          function. Please see description above for how to
 *          write one.
 * @param   platform_parameter
 *			Platform-specific parameter. This parameter is used
 *			to pass information to the library for specific platform.
 *			For most platforms, this should be NULL. For Android,
 *			though, this should contains pointer to the main Activity.
 */
void Run(
    CreateEnvironmentFunc create_environment_func,
    MainLoopFunc          main_loop_func,
    boost::any            platform_parameter);
}

#endif // BLOWGUN_RUN_H
