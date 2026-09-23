#include "../system/SystemAPI.h"

#if defined(_WIN32)
    #define APP_EXPORT __declspec(dllexport)
#elif defined(__linux__) || defined(__APPLE__)
    #define APP_EXPORT __attribute__((visibility("default")))
#else
    // Fallback for standalone Pico 2 OS
    #define APP_EXPORT
#endif

int playerX;
int playerY;
int playerSize;
int speed;

extern "C" 
{
    APP_EXPORT void application_init(SystemAPI* api) 
    {
        playerX    = 50;
        playerY    = 50;
        playerSize = 30;
        speed      = 1;
    }

    APP_EXPORT void application_update(SystemAPI* api) 
    {
        if (api->input->isPressed(Button::RIGHT)) playerX+=speed;
        if (api->input->isPressed(Button::LEFT))  playerX-=speed;
        if (api->input->isPressed(Button::UP))    playerY-=speed;
        if (api->input->isPressed(Button::DOWN))  playerY+=speed;
        
        api->render->drawRectangle(playerX, playerY, playerSize, playerSize, 255);
    }
}