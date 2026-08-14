#include "raylib.h"
#include <vector>
#include "../emulator/RaylibAudioDriver.h"
#include "../sound/MusicAPI.h"

int main()
{
    InitWindow(320, 240, "Modular Audio Sequencer");
    SetTargetFPS(30); 

    AudioBuffer synthBuffer;
    RaylibAudioDriver pcAudio;
    pcAudio.init(&synthBuffer);

    SoundEngine soundEngine;
    MusicAPI music(&soundEngine);

    music.setBPM(90.0f);
    for (int i = 0; i < 4; i++)
    {
        music.setWaveform(i, WaveType::SQUARE);
    }

    std::vector<NoteEvent> menuMusic = {
        // Cmaj7
        {0.0f,  0, Note::C3, 4.0f},
        {0.0f,  1, Note::E3, 4.0f},
        {0.0f,  2, Note::G3, 4.0f},
        {0.0f,  3, Note::B3, 4.0f},

        // Bm7
        {2.0f,  0, Note::C3, 2.0f, true, 0.25f, Note::B2},
        {2.0f,  1, Note::E3, 2.0f, true, 0.25f, Note::D3},
        {2.0f,  2, Note::G3, 2.0f, true, 0.25f, Note::FS3},
        {2.0f,  3, Note::B3, 2.0f, true, 0.25f, Note::A3},
        
        // Cmaj7
        {4.0f,  0, Note::G3, 4.0f, true, 0.25f, Note::C3},
        {4.0f,  1, Note::B3, 4.0f, true, 0.25f, Note::E3},
        {4.0f,  2, Note::D3, 4.0f, true, 0.25f, Note::G3},
        {4.0f,  3, Note::F3, 4.0f, true, 0.25f, Note::B3},
        
        // E7
        {8.0f,  0, Note::C3, 2.0f, true, 0.25f, Note::E3},
        {8.0f,  1, Note::E3, 2.0f, true, 0.25f, Note::G3},
        {8.0f,  2, Note::G3, 2.0f, true, 0.25f, Note::B3},
        {8.0f,  3, Note::B4, 2.0f, true, 0.25f, Note::D4},
        
        // E6
        {10.0f,  0, Note::E3, 2.0f, true, 0.25f, Note::E3},
        {10.0f,  1, Note::G3, 2.0f, true, 0.25f, Note::G3},
        {10.0f,  2, Note::B3, 2.0f, true, 0.25f, Note::B3},
        {10.0f,  3, Note::D4, 2.0f, true, 0.25f, Note::C4},
        
        // Ddim7
        {12.0f,  0, Note::E3, 2.0f, true, 0.25f, Note::D3},
        {12.0f,  1, Note::G3, 2.0f, true, 0.25f, Note::F3},
        {12.0f,  2, Note::B3, 2.0f, true, 0.25f, Note::GS3},
        {12.0f,  3, Note::D4, 2.0f, true, 0.25f, Note::B3},
        
        // Am7
        {14.0f,  0, Note::D3,  4.0f, true, 0.25f, Note::A2},
        {14.0f,  1, Note::F3,  4.0f, true, 0.25f, Note::C3},
        {14.0f,  2, Note::GS3, 4.0f, true, 0.25f, Note::E3},
        {14.0f,  3, Note::B3,  4.0f, true, 0.25f, Note::G3},
    };
    music.loadSong(menuMusic, true);

    while (!WindowShouldClose()) 
    {
        int samplesNeeded = synthBuffer.getFreeSpace();

        for(int i = 0; i < samplesNeeded; i++) 
        {
            music.tickSample();
            synthBuffer.pushSample(soundEngine.getSample());
        }

        pcAudio.update();

        BeginDrawing();
        ClearBackground(DARKBLUE);
        EndDrawing();
    }

    pcAudio.close();
    CloseWindow();
    
    return 0;
}