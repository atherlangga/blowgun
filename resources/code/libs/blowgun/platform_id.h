#define PLATFORM_LINUX      1
#define PLATFORM_WINDOWS    2
#define PLATFORM_ANDROID    3

#if defined(__linux__)
    #if defined(__ANDROID__)
        #define PLATFORM_ID     PLATFORM_ANDROID
    #else
        #define PLATFORM_ID     PLATFORM_LINUX
    #endif
#elif defined(_WIN32)
    #define PLATFORM_ID         PLATFORM_WINDOWS
#endif
