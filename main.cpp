#include <iostream>
#include <vector>

#include "./video/RenderEngine.h"
#include "./sound/AudioBuffer.h"
#include "./sound/SoundEngine.h"
#include "./sound/MusicAPI.h"
#include "./hid/InputManager.h"
#include "./system/SystemAPI.h"

#include "./emulator/raylib/RaylibVideoDriver.h"
#include "./emulator/raylib/RaylibAudioDriver.h"
#include "./emulator/raylib/RaylibInputDriver.h"
#include "./emulator/raylib/RaylibAppLoader.h"

RenderEngine globalRender;
SoundEngine  globalSound;
MusicAPI     globalMusic(&globalSound);
InputManager globalInput;

AudioBuffer       synthBuffer;
RaylibVideoDriver display;
RaylibAudioDriver pcAudio;
RaylibInputDriver pcInput;
ApplcationLoader  appLoader;


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

    #if defined(_WIN32)
        appLoader.loadApplication("games/game.dll", &osAPI);
    #elif defined(__linux__) || defined(__APPLE__)
        appLoader.loadApplication("bin/game.so", &osAPI);
    #else
        // TODO: Path for standalone Pico 2 OS (e.g., loading from flash/SD card)
        appLoader.loadApplication("games/game.bin", &osAPI); 
    #endif

    while (!display.shouldClose()) 
    {
        int samplesNeeded = synthBuffer.getFreeSpace();
        pcInput.update();
        appLoader.update(&osAPI);
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