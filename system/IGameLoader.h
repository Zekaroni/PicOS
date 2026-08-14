#pragma once
#include "../system/SystemAPI.h"

class IGameLoader
{
public:
    virtual ~IGameLoader() = default;

    virtual bool loadGame(const char* filepath, SystemAPI* api) = 0;
    virtual void update(SystemAPI* api) = 0;
    virtual void unloadGame() = 0;
};