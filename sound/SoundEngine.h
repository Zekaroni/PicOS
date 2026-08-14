#pragma once
#include "../sound/Synth.h"

class SoundEngine
{
private:
    static constexpr int MAX_CHANNELS = 5;
    Synth channels[MAX_CHANNELS];

public:
    void playFrequency(int channel, float frequency, float sampleCount = 0)
    {
        if (channel < 0 || channel >= MAX_CHANNELS) return;

        channels[channel].setFrequency(frequency);
        channels[channel].noteOn(sampleCount);
    }
    
    void changeFrequency(int channel, float frequency)
    {
        if (channel < 0 || channel >= MAX_CHANNELS) return;
        channels[channel].setFrequency(frequency);
    }
    
    void stopChannel(int channel)
    {
        if (channel < 0 || channel >= MAX_CHANNELS) return;
        channels[channel].noteOff();
    }

    void slideFrequency(int channel, float frequency, int sampleCount)
    {
        if (channel < 0 || channel >= MAX_CHANNELS) return;
        channels[channel].slideTo(frequency, sampleCount);
    }

    void legatoSlide(int channel, float targetFrequency, int slideSamples, int newDurationSamples)
    {
        if (channel < 0 || channel >= MAX_CHANNELS) return;
        channels[channel].legatoSlide(targetFrequency, slideSamples, newDurationSamples);
    }

    void setChannelWaveform(int channel, WaveType wave)
    {
        if (channel < 0 || channel >= MAX_CHANNELS) return;
        channels[channel].setWaveform(wave);
    }

    void setChannelEnvelope(int channel, float attackBeats, float decayBeats, float sustainLevel, float releaseBeats)
    {
        if (channel < 0 || channel >= MAX_CHANNELS) return;
        channels[channel].setEnvelope(attackBeats, decayBeats, sustainLevel, releaseBeats);
    }


    int16_t getSample()
    {
        int32_t mixedSample = 0;
        int activeChannels  = 0;

        for (int i = 0; i < MAX_CHANNELS; i++)
        {
            if (channels[i].synthActive())
            {
                activeChannels++;
                mixedSample += channels[i].getSample();
            }
        }
        
        if (activeChannels > 0)
        {
            return (int16_t)(mixedSample / activeChannels);
        }
        
        return 0;
    }
};