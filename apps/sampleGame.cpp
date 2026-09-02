#include "../system/SystemAPI.h"

#if defined(_WIN32)
    #define GAME_EXPORT __declspec(dllexport)
#elif defined(__linux__) || defined(__APPLE__)
    #define GAME_EXPORT __attribute__((visibility("default")))
#else
    // Fallback for standalone Pico 2 OS
    #define GAME_EXPORT
#endif

int playerX = 10;
int playerY = 10;

extern "C" 
{
    GAME_EXPORT void application_init(SystemAPI* api) 
    {
        playerX = 50;
        playerY = 50;
    }

    GAME_EXPORT void application_update(SystemAPI* api) 
    {
        if (api->input->isPressed(Button::RIGHT)) playerX+=10;
        if (api->input->isPressed(Button::LEFT))  playerX-=10;
        if (api->input->isPressed(Button::UP))    playerY-=10;
        if (api->input->isPressed(Button::DOWN))  playerY+=10;
        
        api->render->drawRectangle(playerX, playerY, playerX + 10, playerY + 10, 255);
    }
}