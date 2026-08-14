#pragma once
#include "raylib.h"
#include "../sound/AudioBuffer.h"

class RaylibAudioDriver
{
private:
    AudioStream stream;
    AudioBuffer* buffer;

public:
    void init(AudioBuffer* targetBuffer)
    {
        buffer = targetBuffer;
        SetAudioStreamBufferSizeDefault(2048);
        InitAudioDevice();
        stream = LoadAudioStream(AudioBuffer::SAMPLE_RATE, 16, 1);
        PlayAudioStream(stream);
    }

    void update()
    {
        if (IsAudioStreamProcessed(stream))
        {
            const int samplesNeeded = 2048; 
            int16_t tempBuffer[samplesNeeded];
            
            for (int i = 0; i < samplesNeeded; i++)
            {
                tempBuffer[i] = buffer->readSample();
            }
            
            UpdateAudioStream(stream, tempBuffer, samplesNeeded);
        }
    }

    void close()
    {
        UnloadAudioStream(stream);
        CloseAudioDevice();
    }
};