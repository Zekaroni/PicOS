#pragma once
#include <cstdint>
#include <cmath>
#include "../sound/AudioBuffer.h"

enum EnvelopeState { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE };
enum class WaveType { SQUARE, SAW, TRIANGLE, SINE };

class Synth 
{
private:
    WaveType waveform          = WaveType::SINE;
    EnvelopeState state        = IDLE;
    int16_t maxAmplitude       = 20000;
    float currentAmpMultiplier = 0.0f;
    int noteDurationSamples    = 0;
    int noteTimer              = 0;
    float currentPhase         = 0.0f;
    int envelopeTimer          = 0;
    int attackSamples          = 4410;
    int decaySamples           = 4410;
    float sustainLevel         = 0.8f;
    int releaseSamples         = 8820;
    bool hasTimedRelease       = false;

    float currentFrequency     = 0.0f;
    float startFrequency       = 0.0f;
    float endFrequency         = 0.0f;
    int slideSamplesTotal      = 0;
    int slideSamplesElapsed    = 0;
    bool isSliding             = false;

public:

    void noteOn(int durationSamples = 0) 
    {
        state               = ATTACK;
        envelopeTimer       = 0;
        noteTimer           = 0;
        noteDurationSamples = durationSamples;
        hasTimedRelease     = (durationSamples > 0);
        isSliding           = false;
    }

    void noteOff() 
    {
        if (state != IDLE) 
        {
            state = RELEASE;
            envelopeTimer = 0;
        }
        isSliding       = false;
        hasTimedRelease = false;
    }

    void setFrequency(float frequency) 
    {
        currentFrequency = frequency;
    }

    void setWaveform(WaveType type) 
    {
        waveform = type;
    }

    void slideTo(float targetFrequency, int durationSamples)
    {
        startFrequency      = currentFrequency;
        endFrequency        = targetFrequency;
        slideSamplesTotal   = durationSamples;
        slideSamplesElapsed = 0;
        isSliding = (durationSamples > 0);

        if (!isSliding)
        {
            setFrequency(targetFrequency);
        }
    }

    void legatoSlide(float targetFrequency, int slideSamplesTotal, int newDurationSamples)
    {
        slideTo(targetFrequency, slideSamplesTotal);
        noteDurationSamples = newDurationSamples;
        noteTimer = 0;
        hasTimedRelease = (newDurationSamples > 0);
        if (state == RELEASE || state == IDLE) 
        {
            state = SUSTAIN; 
        }
    }

    
    void setEnvelope(int attack, int decay, float sustain, int release)
    {// Everyhing is in samples
        attackSamples   = attack;
        decaySamples    = decay;
        sustainLevel    = sustain;
        releaseSamples  = release;
    }

    int16_t getSample() 
    {
        if (state == IDLE || currentFrequency <= 0.0f) return 0;
        
        if (isSliding)
        {
            slideSamplesElapsed++;
            float progress = (float)slideSamplesElapsed / slideSamplesTotal;

            if (progress >= 1.0f)
            {
                progress = 1.0f;
                isSliding = false;
            }

            float interpolatedFrequency = startFrequency + (endFrequency - startFrequency) * progress;
            setFrequency(interpolatedFrequency);
        }

        if (hasTimedRelease && (state == ATTACK || state == DECAY || state == SUSTAIN)) 
        {
            noteTimer++;
            if (noteTimer >= noteDurationSamples) 
            {
                noteOff();
            }
        }

        if (state == ATTACK) 
        {
            envelopeTimer++;
            currentAmpMultiplier = (float)envelopeTimer / attackSamples;
            if (envelopeTimer >= attackSamples) { state = DECAY; envelopeTimer = 0; }
        }
        else if (state == DECAY) 
        {
            envelopeTimer++;
            float progress = (float)envelopeTimer / decaySamples;
            currentAmpMultiplier = 1.0f - ((1.0f - sustainLevel) * progress);
            if (envelopeTimer >= decaySamples) { state = SUSTAIN; }
        }
        else if (state == SUSTAIN) 
        {
            currentAmpMultiplier = sustainLevel;
        }
        else if (state == RELEASE) 
        {
            envelopeTimer++;
            float progress = (float)envelopeTimer / releaseSamples;
            currentAmpMultiplier = sustainLevel * (1.0f - progress);
            if (envelopeTimer >= releaseSamples || currentAmpMultiplier <= 0.0f) 
            {
                state = IDLE;
                currentAmpMultiplier = 0.0f;
            }
        }

        int16_t rawSample = 0;
        float t = currentPhase;

        switch (waveform) 
        {
            case WaveType::SQUARE:
                rawSample = (t < 0.5f) ? maxAmplitude : -maxAmplitude;
                break;
                
            case WaveType::SAW:
                rawSample = (int16_t)(maxAmplitude * (1.0f - (2.0f * t)));
                break;
                
            case WaveType::TRIANGLE:
                if (t < 0.5f) {
                    rawSample = (int16_t)(-maxAmplitude + (4.0f * maxAmplitude * t));
                } else {
                    rawSample = (int16_t)(3.0f * maxAmplitude - (4.0f * maxAmplitude * t));
                }
                break;
                
            case WaveType::SINE:
                rawSample = (int16_t)(maxAmplitude * std::sin(t * 2.0f * 3.14159265f));
                break;
        }
        
        if (currentFrequency > 0.0f)
        {
            currentPhase += currentFrequency / AudioBuffer::SAMPLE_RATE;
            if (currentPhase >= 1.0f) currentPhase -= 1.0f;
        }

        return (int16_t)(rawSample * currentAmpMultiplier);
    }

    bool synthActive()
    {
        return state != IDLE;
    }
};