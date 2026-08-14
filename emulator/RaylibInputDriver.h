#pragma once
#include "raylib.h"
#include "../hid/InputManager.h" 

class RaylibInputDriver 
{
private:
    InputManager* input;

public:
    void init(InputManager* manager) 
    {
        input = manager;
    }

    void update() 
    {
        input->setButtonState(Button::UP,     IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W));
        input->setButtonState(Button::DOWN,   IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S));
        input->setButtonState(Button::LEFT,   IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A));
        input->setButtonState(Button::RIGHT,  IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D));
        
        input->setButtonState(Button::A,      IsKeyDown(KEY_Z)     || IsKeyDown(KEY_SPACE));
        input->setButtonState(Button::B,      IsKeyDown(KEY_X));
        
        input->setButtonState(Button::START,  IsKeyDown(KEY_ENTER));
        input->setButtonState(Button::SELECT, IsKeyDown(KEY_RIGHT_SHIFT));
    }
};