#pragma once

#if defined(_WIN32)
#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#endif

#include "../system/IGameLoader.h"

class GameLoader: public IGameLoader {
private:
    void* gameModule = nullptr;

public:
    void (*game_init)(SystemAPI*) = nullptr;
    void (*game_update)(SystemAPI*) = nullptr;

    bool loadGame(const char* filepath, SystemAPI* api) {
#if defined(_WIN32)
        gameModule = LoadLibraryA(filepath);
#elif defined(__linux__) || defined(__APPLE__)
        gameModule = dlopen(filepath, RTLD_LAZY);
#else
        // TODO: Standalone Pico 2 OS loader implementation
        gameModule = nullptr; 
#endif

        if (!gameModule) return false;

#if defined(_WIN32)
        game_init = (void(*)(SystemAPI*))GetProcAddress((HMODULE)gameModule, "game_init");
        game_update = (void(*)(SystemAPI*))GetProcAddress((HMODULE)gameModule, "game_update");
#elif defined(__linux__) || defined(__APPLE__)
        game_init = (void(*)(SystemAPI*))dlsym(gameModule, "game_init");
        game_update = (void(*)(SystemAPI*))dlsym(gameModule, "game_update");
#else
        // TODO: Standalone Pico 2 OS symbol resolution
#endif

        if (game_init) game_init(api);

        return true;
    }

    void update(SystemAPI* api) {
        if (game_update) game_update(api);
    }

    void unloadGame() {
        if (gameModule) {
#if defined(_WIN32)
            FreeLibrary((HMODULE)gameModule);
#elif defined(__linux__) || defined(__APPLE__)
            dlclose(gameModule);
#endif
        }

        gameModule  = nullptr;
        game_init   = nullptr;
        game_update = nullptr;
    }
};