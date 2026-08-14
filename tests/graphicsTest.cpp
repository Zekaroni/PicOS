#include <iostream>
#include "../video/RenderEngine.h"
#include "../emulator/RaylibVideoDriver.h"

int main()
{
    RenderEngine renderEngine;
    RaylibVideoDriver display;

    display.init();
    int circleX    =  0;

    display.showFPS = false;
    
    while (!display.shouldClose())
    {
        renderEngine.drawCircle(circleX, 100, 50, 5, 0b11000);
        renderEngine.drawText(0, 0, "!\"#$%&'()*+'-./:;<=>?@[]^_`{|}~", 2, 0b11100000);
        renderEngine.drawText(0, 8, "0123456789", 2, 0b11100000);
        renderEngine.drawText(0, 16, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 2, 0b11100000);
        renderEngine.drawText(0, 24, "abcdefghijklmnopqrstuvwxyz", 2, 0b11100000);

        
        circleX++;
        if(circleX > renderEngine.SCREEN_WIDTH) circleX = 0;

        display.render(renderEngine);
        renderEngine.clear();
    }

    display.close();
    return 0;
}