#include <iostream>
#include <vector>

#include "./video/RenderEngine.h"
#include "./sound/AudioBuffer.h"
#include "./sound/SoundEngine.h"
#include "./sound/MusicAPI.h"
#include "./hid/InputManager.h"
#include "./system/SystemAPI.h"

#include "./emulator/RaylibVideoDriver.h"
#include "./emulator/RaylibAudioDriver.h"
#include "./emulator/RaylibInputDriver.h"
#include "./emulator/RaylibGameLoader.h"

RenderEngine globalRender;
SoundEngine globalSound;
MusicAPI globalMusic(&globalSound);
InputManager globalInput;

AudioBuffer synthBuffer;
RaylibVideoDriver display;
RaylibAudioDriver pcAudio;
RaylibInputDriver pcInput;
GameLoader cartLoader;


SystemAPI buildAPI() 
{
    SystemAPI api;
    api.render = &globalRender;
    api.sound  = &globalSound;
    api.music  = &globalMusic;
    api.input  = &globalInput;
    return api;
}

int main() 
{
    display.init();
    pcAudio.init(&synthBuffer);
    pcInput.init(&globalInput);

    display.showFPS = false;

    SystemAPI osAPI = buildAPI();

    cartLoader.loadGame("games/game.dll", &osAPI);

    while (!display.shouldClose()) 
    {
        int samplesNeeded = synthBuffer.getFreeSpace();
        pcInput.update();
        cartLoader.update(&osAPI);
        display.render(globalRender);
        for (int i = 0; i < samplesNeeded; i++) 
        {
            globalMusic.tickSample();
            synthBuffer.pushSample(globalSound.getSample());
        }
        pcAudio.update();

        globalRender.clear();
        globalInput.update();
    }

    pcAudio.close();
    display.close();

    return 0;
}