#include "../system/SystemAPI.h"

RenderEngine globalRenderEngine;
SoundEngine  globalSound;
MusicAPI     globalMusic(&globalSound);
InputManager inputManager;

// TODO: Implement a "virtual" system where code being executed will not break the machine if
//       there is an error in the current app. This may also allow for "unloading" the current
//       app, but keep the same state when reopening. Think the 3DS menu how you can back out
//       to the menu.

SystemAPI buildAPI() 
{
    SystemAPI api;
    
    api.render = &globalRenderEngine;
    api.sound  = &globalSound;
    api.music  = &globalMusic;
    api.input  = &inputManager;

    return api;
}
