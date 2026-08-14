#pragma once
#include <cstdint>
#include "../video/RenderEngine.h"
#include "../sound/SoundEngine.h"
#include "../sound/MusicAPI.h"
#include "../hid/InputManager.h"


struct SystemAPI 
{
    RenderEngine* render;
    SoundEngine*  sound;
    MusicAPI*     music;
    InputManager* input;
};