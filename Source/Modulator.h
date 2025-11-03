/*
  ==============================================================================

    Modulator.h
    Created: 23 Oct 2025 2:17:00pm
    Author:  BM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class Modulator {
public:
    virtual ~Modulator() = default;
    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) { this->sampleRate = sampleRate; }
    virtual float getModulatedCutoff(float cutoff, int samplesPerBlock) = 0;  
    virtual void setRate(float rateHz) { rate = rateHz; }
    //virtual void syncToBPM(double bpm) = 0;

protected:
    double sampleRate = 44100.0;
    float rate = 1.0f; //hz
};


class WaveGen {
    const float PI = 3.14159265358f;
public:
    enum WaveType { Sine, Square, Triangle, Saw };

    void setFrequency(float newFreq, float newSampleRate)
    {
        freq = newFreq;
        sampleRate = newSampleRate;
        phaseIncrement = (2.0f * PI * freq) / sampleRate;
    }

    void setWaveType(WaveType type)
    {
        waveType = type;
    }

    void reset()
    {
        phase = 0.0f;
    }

    float getNextValue()
    {
        // increment phase
        phase += phaseIncrement;
        if (phase >= 2.0f * juce::MathConstants<float>::pi)
            phase -= 2.0f * juce::MathConstants<float>::pi;

        // generate waveform
        switch (waveType)
        {
        case WaveType::Sine:
            return std::sin(phase);

        case WaveType::Square:
            return (std::sin(phase) >= 0.0f ? 1.0f : -1.0f);

        case WaveType::Triangle:
            return (2.0f / juce::MathConstants<float>::pi) * std::asin(std::sin(phase));

        case WaveType::Saw:
            return (1.0f / juce::MathConstants<float>::pi) * (phase - juce::MathConstants<float>::pi);
        }

        return 0.0f;
    }

private:
    float freq = 1.0f;
    float sampleRate = 44100.0f;
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    WaveType waveType = WaveType::Sine;
};

class Oscillator : public Modulator{
public:
    void setWaveType(WaveGen::WaveType type);
    void setDepth(float depthAmount);
    float getModulatedCutoff(float cutoff, int samplesPerBlock) override;

private:
    float depth = 1.0f;
    //juce::dsp::Oscillator<float> osc;  
};

class StepSequencer : public Modulator {
public:
    StepSequencer() {
        for (int i = 0; i < 16; i++) stepValues[i] = 500.f;

        samplesPerHalfCycle = static_cast<int>(0.5f * sampleRate / rate / numActiveSteps);
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock);

    float getModulatedCutoff(float cutoff, int samplesPerBlock) override;
    
    void setRate(float rateHz) override;

    void setFilter(int index, float cutoff);
    
    void setNumActiveSteps(int n);
private:
    void updateHalfCycle() {
        samplesPerHalfCycle = static_cast<int>(0.5f * sampleRate / rate);
    }

    std::array<float, 16> stepValues{};

    unsigned long long sampleCount = 0;

    int numActiveSteps = 2;
    int currentStep = 0;
    float rate = 1.f; 
    int samplesPerHalfCycle;
};

