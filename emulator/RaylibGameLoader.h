#pragma once

#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../system/IGameLoader.h"

class GameLoader: public IGameLoader
{
private:
    HMODULE gameModule = nullptr;

public:
    void (*game_init)(SystemAPI*) = nullptr;
    void (*game_update)(SystemAPI*) = nullptr;

    bool loadGame(const char* filepath, SystemAPI* api) 
    {
        gameModule = LoadLibraryA(filepath);
        if (!gameModule) return false;
        game_init = (void(*)(SystemAPI*))GetProcAddress(gameModule, "game_init");
        game_update = (void(*)(SystemAPI*))GetProcAddress(gameModule, "game_update");
        if (game_init) game_init(api);
        
        return true;
    }

    void update(SystemAPI* api) 
    {
        if (game_update) game_update(api);
    }

    void unloadGame() 
    {
        if (gameModule) FreeLibrary(gameModule);
        gameModule  = nullptr;
        game_init   = nullptr;
        game_update = nullptr;
    }
};