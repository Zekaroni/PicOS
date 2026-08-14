#pragma once
#include <cstdint>

class AudioBuffer
{
public:
    static constexpr int SAMPLE_RATE = 44100;
    static constexpr int BUFFER_SIZE = 4096;

private:
    int16_t ringBuffer[BUFFER_SIZE];
    int writeIndex = 0;
    int readIndex = 0;

public:
    int getFreeSpace()
    {
        int space = readIndex - writeIndex - 1;
        if (space < 0) space += BUFFER_SIZE;
        return space;
    }

    void pushSample(int16_t sample)
    {
        ringBuffer[writeIndex] = sample;
        writeIndex = (writeIndex + 1) % BUFFER_SIZE;
    }

    int16_t readSample()
    {
        if (readIndex == writeIndex) return 0;
        
        int16_t sample = ringBuffer[readIndex];
        readIndex = (readIndex + 1) % BUFFER_SIZE;
        return sample;
    }
};