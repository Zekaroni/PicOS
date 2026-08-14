#pragma once
#include <vector>
#include "../sound/SoundEngine.h"

namespace Note 
{
    constexpr uint8_t REST = 0; 
    constexpr uint8_t A2  = 45;
    constexpr uint8_t AS2  = 46;
    constexpr uint8_t B2  = 47;
    constexpr uint8_t C3  = 48;
    constexpr uint8_t CS3 = 49;
    constexpr uint8_t D3  = 50;
    constexpr uint8_t DS3 = 51;
    constexpr uint8_t E3  = 52;
    constexpr uint8_t F3  = 53;
    constexpr uint8_t FS3 = 54;
    constexpr uint8_t G3  = 55;
    constexpr uint8_t GS3 = 56;
    constexpr uint8_t A3  = 57;
    constexpr uint8_t AS3 = 58;
    constexpr uint8_t B3  = 59;
    constexpr uint8_t C4  = 60;
    constexpr uint8_t CS4 = 61; 
    constexpr uint8_t D4  = 62;
    constexpr uint8_t DS4 = 63; 
    constexpr uint8_t E4  = 64;
    constexpr uint8_t F4  = 65;
    constexpr uint8_t FS4 = 66; 
    constexpr uint8_t G4  = 67;
    constexpr uint8_t GS4 = 68; 
    constexpr uint8_t A4  = 69;
    constexpr uint8_t AS4 = 70; 
    constexpr uint8_t B4  = 71;
    constexpr uint8_t C5  = 72;
    constexpr uint8_t CS5 = 73;
    constexpr uint8_t D5  = 74;
    constexpr uint8_t DS5 = 75;
    constexpr uint8_t E5  = 76;
    constexpr uint8_t F5  = 77;
    constexpr uint8_t FS5 = 78;
    constexpr uint8_t G5  = 79;
    constexpr uint8_t GS5 = 80;
    constexpr uint8_t A5  = 81;
    constexpr uint8_t AS5 = 82;
    constexpr uint8_t B5  = 83;
    constexpr uint8_t C6  = 84;
    constexpr uint8_t CS6  = 85;
    constexpr uint8_t D6  = 86;
}

// startBeat (f), channel (i), midi(uint8), duration(f), slide(b), target(f)
struct NoteEvent
{ 
    float startBeat;
    int   channel;
    uint8_t midiNote;
    float durationBeats;
    bool isSlide = false;
    float sldieBeatDuration = 0.0f;
    uint8_t targetMidiNote = 0;
};

class MusicAPI
{
private:
    SoundEngine* engine;
    std::vector<NoteEvent> activeSong;
    float currentBPM        = 120.0f;
    int currentNoteIndex    = 0;
    int songLengthSamples   = 0;
    int totalSamplesElapsed = 0;
    bool isPlayingSong      = false;
    bool isLooping          = false;
    
    int beatsToSamples(float beats)
    {
        if (!beats) return 0;
        float secondsPerBeat = 60.0f / currentBPM;
        return static_cast<int>(beats * secondsPerBeat * AudioBuffer::SAMPLE_RATE);
    }


    float midiToFreq(uint8_t midiNote)
    {
        if (midiNote == 0) return 0.0f; // Handle rest notes
        return 440.0f * std::pow(2.0f, (midiNote - 69.0f) / 12.0f);
    }

public:
    MusicAPI(SoundEngine* soundEngine)
    {
        engine = soundEngine;
    }
    
    void setBPM(float newBPM)
    {
        if (newBPM > 0 && newBPM < 1000) currentBPM = newBPM;
    }

    void playNote(int channel, uint8_t midiNote, float beatDuration)
    {
        float frequency = midiToFreq(midiNote);
        engine->playFrequency(channel, frequency, beatsToSamples(beatDuration));
    }

    void glissando(int channel, uint8_t endMidiNote, float beatCount)
    {
        float targetFrequency = midiToFreq(endMidiNote);
        engine->slideFrequency(channel, targetFrequency, beatsToSamples(beatCount));
    }

    void legato(int channel, uint8_t endMidiNote, float slideBeats, float durationBeats)
    {
        float targetFrequency = midiToFreq(endMidiNote);
        engine->legatoSlide(channel, targetFrequency, beatsToSamples(slideBeats), beatsToSamples(durationBeats));
    }

    void setEnvelope(int channel, float attackBeats, float decayBeats, float sustainLevel, float releaseBeats)
    {
        int attackSamples  = beatsToSamples(attackBeats);
        int decaySamples   = beatsToSamples(decayBeats);
        int releaseSamples = beatsToSamples(releaseBeats);

        engine->setChannelEnvelope(channel, attackSamples, decaySamples, sustainLevel, releaseSamples);
    }

    void setWaveform(int channel, WaveType waveform)
    {
        engine->setChannelWaveform(channel, waveform);
    }


    void loadSong(std::vector<NoteEvent> newSong, bool loop = false)
    {
        activeSong          = newSong;
        currentNoteIndex    = 0;
        totalSamplesElapsed = 0;
        isPlayingSong       = true;
        isLooping           = loop;

        float finalBeat = 0.0f;
        for (const auto& note: activeSong)
        {
            float endBeat = note.startBeat + note.durationBeats;
            if (endBeat > finalBeat) finalBeat = endBeat;
        }
        songLengthSamples = beatsToSamples(finalBeat);
    }

    bool isFinished()
    {
        return !isPlayingSong && !isLooping;
    }

    void tickSample()
    {
        if (!isPlayingSong || activeSong.empty()) return;

        if (totalSamplesElapsed >= songLengthSamples)
        {
            if (isLooping)
            {
                totalSamplesElapsed = 0;
                currentNoteIndex    = 0;
            } else
            {
                isPlayingSong = false;
                return;
            }
        }

        while (currentNoteIndex < activeSong.size() && 
               totalSamplesElapsed >= beatsToSamples(activeSong[currentNoteIndex].startBeat))
        {
            NoteEvent nextNote = activeSong[currentNoteIndex];
            
            if (nextNote.isSlide)
            {
                legato(nextNote.channel, nextNote.targetMidiNote, nextNote.sldieBeatDuration, nextNote.durationBeats);
            } 
            else
            {
                playNote(nextNote.channel, nextNote.midiNote, nextNote.durationBeats);
            }
            
            currentNoteIndex++;
        }

        totalSamplesElapsed++;
    }
};