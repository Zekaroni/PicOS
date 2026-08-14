#include "../system/SystemAPI.h"

int playerX = 10;
int playerY = 10;

extern "C" 
{
    __declspec(dllexport) void game_init(SystemAPI* api) 
    {
        playerX = 50;
        playerY = 50;
    }

    __declspec(dllexport) void game_update(SystemAPI* api) 
    {
        if (api->input->isPressed(Button::RIGHT)) playerX++;
        if (api->input->isPressed(Button::LEFT))  playerX--;
        if (api->input->isPressed(Button::UP))    playerY--;
        if (api->input->isPressed(Button::DOWN))  playerY++;
        api->render->drawRectangle(playerX, playerY, playerX + 10, playerY + 10, 255);
    }
}