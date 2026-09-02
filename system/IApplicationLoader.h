#pragma once
#include "../system/SystemAPI.h"

class IApplicationLoader
{
public:
    virtual ~IApplicationLoader() = default;

    virtual bool loadApplication(const char* filepath, SystemAPI* api) = 0;
    virtual void update(SystemAPI* api) = 0;
    virtual void unloadApplication() = 0;
};