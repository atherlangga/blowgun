#ifndef BLOWGUN_ENVIRONMENT_DEFAULT_H
#define BLOWGUN_ENVIRONMENT_DEFAULT_H

#include "environment.h"

namespace blowgun
{

typedef std::map<EGLint, EGLint> ConfigAttributes;
typedef std::map<EGLint, EGLint> ContextAttributes;

typedef std::function<ConfigAttributes (void)>  CreateConfigAttributesFunc;
typedef std::function<ContextAttributes (void)> CreateContextAttributesFunc;

/**
 * Factory method to take care the instantiation of
 * `blowgun::Environment`.
 */
std::unique_ptr<Environment>
CreateEnvironmentDefault(
    CreateNativeInterfaceFunc   create_native_interface_func,
    CreateConfigAttributesFunc  create_config_attributes_func,
    CreateContextAttributesFunc create_context_attributes_func
);

/**
 * Wrapper for CreateEnvironmentDefault.
 */
typedef std::function
    <std::unique_ptr
        <const Environment>
        (CreateNativeInterfaceFunc,
         CreateConfigAttributesFunc,
         CreateContextAttributesFunc)>
CreateEnvironmentDefaultFunc;

} // namespace blowgun

#endif // BLOWGUN_ENVIRONMENT_DEFAULT_H
