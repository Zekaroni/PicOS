#include "../system/SystemAPI.h"

RenderEngine globalRenderEngine;
SoundEngine  globalSound;
MusicAPI     globalMusic(&globalSound);
InputManager inputManager;

SystemAPI buildAPI() 
{
    SystemAPI api;
    
    api.render = &globalRenderEngine;
    api.sound  = &globalSound;
    api.music  = &globalMusic;
    api.input  = &inputManager;

    return api;
}