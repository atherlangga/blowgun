#include "native_interface.h"

#if defined(__ANDROID__)

#include <iostream>
#include <memory>

using namespace blowgun;

std::unique_ptr<const NativeInterface>
blowgun::CreateNativeInterface()
{
    // TODO: As of now, native window and native display for Android are
    // constructed in Environment. That's actually ugly.
    auto native_interface = new NativeInterface;
    return std::unique_ptr<const NativeInterface>(native_interface);
}

#endif // defined(__ANDROID__)
