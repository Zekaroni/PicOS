#include "../system/SystemAPI.h"

#if defined(_WIN32)
    #define GAME_EXPORT __declspec(dllexport)
#elif defined(__linux__) || defined(__APPLE__)
    #define GAME_EXPORT __attribute__((visibility("default")))
#else
    #define GAME_EXPORT
#endif


extern "C" 
{
    GAME_EXPORT void application_init(SystemAPI* api) 
    {
        // Ran when the game is first ran
    }

    GAME_EXPORT void application_update(SystemAPI* api) 
    {
        // Main game loop
    }
}