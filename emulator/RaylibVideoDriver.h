#pragma once
#include "raylib.h"
#include "../video/RenderEngine.h"

class RaylibVideoDriver
{
public:
    bool showFPS = true;

    void init()
    {
        SetTraceLogLevel(LOG_NONE);
        InitWindow(RenderEngine::SCREEN_WIDTH, RenderEngine::SCREEN_HEIGHT, "Pi Pico OS Emulator");
        SetTargetFPS(30);
    }

    void render(RenderEngine& renderEngine)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        for (int y = 0; y < RenderEngine::SCREEN_HEIGHT; y++)
        {
            for (int x = 0; x < RenderEngine::SCREEN_WIDTH; x++)
            {
                uint8_t bufferColor = renderEngine.displayBuffer[y][x];
                if (bufferColor != 0)
                {
                    DrawPixel(x, y, (Color){
                        (unsigned char)(((bufferColor >> 5) & 0b111) * 255 / 7), 
                        (unsigned char)(((bufferColor >> 2) & 0b111) * 255 / 7), 
                        (unsigned char)((bufferColor & 0b11) * 255 / 3),
                        255
                    });
                }
            }
        }
        renderEngine.swapBuffers();
        if (showFPS) DrawFPS(0, 0);
        EndDrawing();
    }

    bool shouldClose() 
    {
        return WindowShouldClose();
    }

    void close()
    {
        CloseWindow();
    }
};