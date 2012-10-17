#ifndef BLOWGUN_RUN_H
#define BLOWGUN_RUN_H

#include <functional>
#include <memory>

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
 * Here's an example for illustrative purpose:
 *
 * <pre>
 * {@code
 *
 *     DoTheInitialization();
 *
 *     // Start the magic.
 *     while (!platform->IsExitRequested)
 *     {
 *         platform->OnPreFrame();
 *
 *         DoAwesomeStuffs();
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
 *          function. Please see description above to know how to
 *          write one.
 */
void Run(
    CreateEnvironmentFunc create_environment_func,
    MainLoopFunc          main_loop_func,
    void*                 platform_parameter);
}

#endif // BLOWGUN_RUN_H
