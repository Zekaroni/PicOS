#pragma once
#include <cstdint>

enum class Button
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    A,
    B,
    START,
    SELECT,
    MAX_BUTTONS
};

class InputManager
{
private:
    bool currentFrame[(int)Button::MAX_BUTTONS] = {false};
    bool lastFrame[(int)Button::MAX_BUTTONS] = {false};

public:
    void setButtonState(Button btn, bool isDown)
    {
        currentFrame[(int)btn] = isDown;
    }

    void update()
    {
        for (int i = 0; i < (int)Button::MAX_BUTTONS; i++)
        {
            lastFrame[i] = currentFrame[i];
        }
    }

    bool isPressed(Button btn)
    {
        return currentFrame[(int)btn];
    }

    bool isJustPressed(Button btn)
    {
        return currentFrame[(int)btn] && !lastFrame[(int)btn];
    }
};