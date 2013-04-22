#ifndef BLOWGUN_PLATFORM_H
#define BLOWGUN_PLATFORM_H

#include <functional>
#include <memory>
#include <boost/any.hpp>

#include "environment.h"
#include "run.h"

namespace blowgun
{

/**
 * The representation of underlying Platform. The main task of this
 * class is to become a gateway for the application to access common
 * platform-specific functionality so user doesn't have to deal with
 * platform-dependent code.
 *
 * Being a wrapper for platform-specific functionality, this class will
 * create unique instance of both NativeInterface and Environment. It
 * does so by using the functions passed via `blowgun::Run` from the
 * user.
 */
class Platform
{
public:

    /**
     * Prepares the underlying platform for the application to start
     * its frame processing smoothly. One of the thing that will
     * happen under the hood is to pump for events from end-user.
     *
     * User needs to make sure that this method is called before
     * frame processing, otherwise blowgun won't guarantee the
     * user's application to run well.
     */
    void OnPreFrame();

    /**
     * Makes sure that the underlying platform does its job so user's
     * application behave properly after frame processing. One of the
     * thing that will be done under the hood in this method is to
     * swap the graphics buffer.
     *
     * User needs to make sure that this method is called when frame
     * processing is done.
     */
    void OnPostFrame();

    /**
     * Checks whether the underlying platform wants user's application
     * to be finished and dies properly. For example, in Windows and
     * Linux (with X11) platform, this method will return true if the
     * end-user click application's close button.
     *
     * User's application needs to check the return value of this method
     * in order to die gracefully. Most likely this method will become
     * a condition in the main loop. For example:
     *
     *     while (!platform->IsExitRequested)
     *     {
     *         // .. do the frame processing.
     *     }
     */
    bool IsExitRequested() const;

    /**
     * Public destructor needed for PIMPL.
     */
    ~Platform();

private:
    /**
     * Private constructor. Should only be called from `blowgun::Run`.
     */
    explicit Platform(
        CreateEnvironmentFunc create_environment_func,
        boost::any            param);

    /**
     * Prepare the underlying platform.
     */
    void Initialize();

    /**
     * Clean up the platform.
     */
    void Shutdown();

private:
    ///
    // The thingies needed for PIMPL idiom.
    ///
    class Impl;
    std::unique_ptr<Impl> impl_;

/**
 * Only blowgun::Run that allowed to construct, initialize, and shutdown
 * the platform.
 */
friend void Run(CreateEnvironmentFunc, MainLoopFunc, boost::any);
};

}

#endif // BLOWGUN_PLATFORM_H
