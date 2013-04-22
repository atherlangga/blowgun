################################################################################
# Set up the CMake itself
################################################################################

cmake_minimum_required (VERSION 2.8.7)

project (blowgun)

# Set CMake behavior
# Here's what cmake says about CMP0015:
# `link_directories() treats paths relative to the source dir.`
cmake_policy (SET CMP0015 NEW)


################################################################################
# Functions definition
################################################################################

# ----------------------------------------------------------------------
# join_arguments (var)
#
# Join the arguments in the (semi-colon separated) list VAR into one
# space separated string. The string will be returned in the variable
# VAR. This command is the opposite of the built-in command
# `separate_arguments()`.
# ----------------------------------------------------------------------
function (join_arguments var)
    set (_var)
    foreach (_v ${${var}})
        set (_var "${_var} ${_v}")
    endforeach ()
    string (STRIP ${_var} _var)
    set (${var} ${_var})
endfunction ()

# ----------------------------------------------------------------------
# generate_project (project_dir_path project_sources)
#
# Automatically generate hierarchical source groups. This will be useful
# to produce neat-looking Visual Studio project.
# ----------------------------------------------------------------------
function (generate_project project_dir project_sources)
    set (dir_sources "${project_sources}")
    foreach (source ${dir_sources})
        string (REGEX REPLACE "${project_dir}" "" relative_path
            "${source}")
        string (REGEX REPLACE "[\\\\/][^\\\\/]*$" "" relative_path
            "${relative_path}")
        string (REGEX REPLACE "^[\\\\/]" "" relative_path
            "${relative_path}")
        string (REGEX REPLACE "/" "\\\\\\\\" relative_path
            "${relative_path}")
        source_group ("${relative_path}" FILES ${source})
    endforeach ()
endfunction ()

# ----------------------------------------------------------------------
# add_copy_files_target (target_name base_input_dir base_output_dir)
#
# Add a target which its job is to copy each files in `base_input_dir`
# to `base_output_dir`.
# ----------------------------------------------------------------------
function (add_copy_files_target target_name base_input_dir base_output_dir)
    # Initialize the output variable
    set (output_files_list)

    # Traverse all the needed files
    file (
        GLOB_RECURSE
        input_files
        ${base_input_dir}/*)

    # Process every found file
    foreach (input_file ${input_files})
        # Determine the output file name
        string (
            REGEX REPLACE
            "${base_input_dir}"
            "${base_output_dir}"
            output_file
            ${input_file})

        # Insert the output file name to the output varible
        list (APPEND output_files_list ${output_file})

        # Copy that file if it's changed
        add_custom_command(
            OUTPUT ${output_file}
            COMMAND
                ${CMAKE_COMMAND} -E copy
                ${input_file}
                ${output_file})
    endforeach ()

    # Name the job
    add_custom_target(
        ${target_name}
        DEPENDS ${output_files_list})
endfunction()


################################################################################
# Set up project-specific variables
################################################################################

# Defines the "good-default" Target OS
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    if (ANDROID)
        set (target_os_default "Android")
    else ()
        set (target_os_default "Linux")
    endif ()
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set (target_os_default "Windows")
endif ()

set (target_os ${target_os_default} CACHE STRING "Target OS")

if (NOT (target_os MATCHES "Linux")
    AND
    NOT (target_os MATCHES "Windows")
    AND
    NOT (target_os MATCHES "Android"))
    message (FATAL_ERROR "Unsupported Target OS: ${target_os}")
endif ()


################################################################################
# Set up CMake variables
################################################################################

if (NOT CMAKE_BUILD_TYPE)
    # If build type isn't set, set it to "Debug"
    set (CMAKE_BUILD_TYPE "Debug")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.6")
        message (FATAL_ERROR "This project requires GNU C++ Compiler version 4.6 or later")
    endif ()
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU"
    OR
    CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Set GCC and Clang configuration
    set (CMAKE_CXX_FLAGS
        "-pedantic -Wall -Wextra -Weffc++ -Woverloaded-virtual -fexceptions")
    set (CMAKE_CXX_FLAGS_DEBUG
        "-O0 -g")
    set (CMAKE_CXX_FLAGS_RELEASE
        "-O3 -s")

    if (target_os MATCHES "Linux")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    elseif (target_os MATCHES "Android")
        # As of version 7 of Crystax NDK, C++11 feature is enabled with
        # `gnu++0x` instead of `c++0x` or `c++11`.
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
    endif ()
endif ()


################################################################################
# Set up the environment for the toolchain
################################################################################

if (target_os MATCHES "Linux")
    # UNIX platform needs X11 to work
    find_package (X11 REQUIRED)
    include_directories (AFTER ${X11_INCLUDE_DIR})

    # Hack for `eglplatform.h` included in PowerVR SDK
    add_definitions ("-DSUPPORT_X11")
endif ()

# ----------------------------------------------------------------------

if (target_os MATCHES "Linux" OR target_os MATCHES "Windows")
    # Set include directory for OpenGL ES 2 ..
    include_directories (AFTER
        "resources/third-party/PowerVR/Builds/OGLES2/Include")

    # .. and its static library directory
    if (target_os MATCHES "Linux")
        link_directories (
            "resources/third-party/PowerVR/Builds/OGLES2/LinuxX86/Lib")
    elseif (target_os MATCHES "Windows")
        link_directories (
            "resources/third-party/PowerVR/Builds/OGLES2/WindowsX86/Lib")
    endif ()
elseif (target_os MATCHES "Android")
    # By default, all Android devices have OpenGL ES embedded in it. So,
    # no need to define anything for Android because OpenGL ES subsystem
    # (almost) guaranteed to be exist.
endif ()

# ----------------------------------------------------------------------

if (MSVC_IDE)
    # Due to following facts:
    # - Visual Studio often put the resulting build files and executable
    #   in each configuration-specific directory instead of one common
    #   directory.
    # - Win32 executable needs `libEGL.dll` and `libGLESv2.dll` to be
    #   able to run.
    # We have an issue: how to make sure the application finds those
    # dlls? There are few options:
    # - Copy those dlls to one of the global paths (such as
    #   C:\Windows\System32), but the practice littering global path
    #   just for one application to run definitely not recommended.
    # - Copy the dlls to the all configurations directory. So, wherever
    #   the executable located by Visual Studio, those dlls will be
    #   waiting for it.
    # - Configure Visual Studio to put the executable on one single
    #   location regardless of the configuration. And then copy the the
    #   dlls onto that location.
    # The latter option is the best option. However, I don't know how
    # to achieve that with CMake :p. So, I prefer the second option for
    # the time being.
    foreach (VS_CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
        file (
            COPY "resources/third-party/PowerVR/Builds/OGLES2/WindowsX86/Lib/"
            DESTINATION "${CMAKE_BINARY_DIR}/${VS_CONFIG_TYPE}"
            FILES_MATCHING PATTERN *.dll)
    endforeach ()
endif ()

# ----------------------------------------------------------------------

if (target_os MATCHES "Android")
    # Copy the necessary files to create bare-mininum Android
    # application.
    # TODO: Detect changes on the source directory so the destination
    # will be updated automatically. The way it is now is to do it
    # manually.
    add_copy_files_target (
        android_project_files
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/code/os_bootstrap/android/
        ${CMAKE_CURRENT_BINARY_DIR}/)
endif ()

# ----------------------------------------------------------------------

# Copy the application data
add_copy_files_target (
    static_resources_files
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/data/
    ${CMAKE_CURRENT_BINARY_DIR}/data/)


################################################################################
# Set up all the projects
################################################################################

include_directories (AFTER "resources/code/libs")

# ----------------------------------------------------------------------

# Build Blowgun
# Blowgun is the wrapper for OpenGL ES 2.
file (GLOB_RECURSE blowgun_hdrs "resources/code/libs/blowgun/*.h*")
file (GLOB_RECURSE blowgun_srcs "resources/code/libs/blowgun/*.c*")
add_library (blowgun ${blowgun_hdrs} ${blowgun_srcs})

file (GLOB_RECURSE blowgun_files "resources/code/libs/blowgun/*")
generate_project (
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/code/libs/blowgun"
    "${blowgun_files}")

# ----------------------------------------------------------------------

# Build Logog
# Logog is used to handle application logging.
file (GLOB_RECURSE logog_hdrs "resources/code/libs/logog/*.h*")
file (GLOB_RECURSE logog_srcs "resources/code/libs/logog/*.c*")
add_library (logog ${logog_hdrs} ${logog_srcs})

file (GLOB_RECURSE logog_files "resources/code/libs/logog/*")
generate_project (
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/code/libs/logog"
    "${logog_files}")

# ----------------------------------------------------------------------

# Build the application
file (GLOB_RECURSE application_hdrs "resources/code/application/*.h*")
file (GLOB_RECURSE application_srcs "resources/code/application/*.c*")
add_library (application ${application_hdrs} ${application_srcs})

file (GLOB_RECURSE application_files "resources/code/application/*")
generate_project (
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/code/application"
    "${application_files}")

# ----------------------------------------------------------------------

if ((target_os MATCHES "Linux") OR (target_os MATCHES "Windows"))
    # For Linux and Windows, the simple bootstrapper can be used.
    file (GLOB_RECURSE os_bootstrap_hdrs
        "resources/code/os_bootstrap/default/*.h*")
    file (GLOB_RECURSE os_bootstrap_srcs
        "resources/code/os_bootstrap/default/*.c*")
    add_executable (os_bootstrap ${os_bootstrap_hdrs} ${os_bootstrap_srcs})
elseif (target_os MATCHES "Android")
    # For native Android application, it has to be a shared library
    file (GLOB_RECURSE os_bootstrap_hdrs
        "resources/code/os_bootstrap/android/*.h*")
    file (GLOB_RECURSE os_bootstrap_srcs
        "resources/code/os_bootstrap/android/*.c*")
    add_library (os_bootstrap SHARED ${os_bootstrap_hdrs} ${os_bootstrap_srcs})
    add_dependencies (os_bootstrap android_project_files)
endif ()

add_dependencies (os_bootstrap static_resources_files)

# Set the final output filename to be more descriptive.
# By default, the final output name is "os_bootstrap", which doesn't
# describe itself very well.
set (SAMPLE_APP_NAME "${PROJECT_NAME}_sampleapp")
set_target_properties (os_bootstrap PROPERTIES OUTPUT_NAME ${SAMPLE_APP_NAME})

file (GLOB_RECURSE os_bootstrap_files "resources/code/os_bootstrap/*")
generate_project (
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/code/os_bootstrap"
    "${os_bootstrap_files}")

# ----------------------------------------------------------------------

if (target_os MATCHES "Android")
    # Set the output directory for Android's shared library.
    # TODO: the architecture is hardcoded to armeabi. It should be
    # dynamically chosen based on shared library's architecture type.
    set (android_output_directory "${CMAKE_BINARY_DIR}/libs/armeabi")
    set_target_properties (os_bootstrap PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${android_output_directory})

    # If that output directory is not exist yet, create it
    if (NOT EXISTS "${android_output_directory}")
        execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
            ${android_output_directory})
    endif ()
endif ()

# ----------------------------------------------------------------------

# Linking
# Aside from the linking itself, this part also contains
# the dependency information.
if (target_os MATCHES "Linux")
    target_link_libraries (application EGL GLESv2 ${X11_LIBRARIES} rt)
elseif (target_os MATCHES "Windows")
    target_link_libraries (application libEGL libGLESv2)
elseif (target_os MATCHES "Android")
    target_link_libraries (application EGL GLESv2 log android)
endif ()

target_link_libraries (os_bootstrap application blowgun logog)


################################################################################
# Set up testing
################################################################################

find_package (GTest)

if (GTEST_FOUND)
    enable_testing()
    include_directories (${GTEST_INCLUDE_DIRS})

    file (GLOB_RECURSE blowgun_test_files "resources/code/libs/blowgun/*_test.c*")
    set (TEST_APP_NAME "${PROJECT_NAME}_testrunner")
    add_executable (${TEST_APP_NAME} ${blowgun_test_files})
    target_link_libraries(${TEST_APP_NAME} blowgun ${GTEST_BOTH_LIBRARIES})
endif (GTEST_FOUND)
