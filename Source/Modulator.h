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
    virtual float setCutoff(float cutoff) = 0;  //in [0,1]
    void setRate(float rateHz);
    void syncToBPM(double bpm);

protected:
    double sampleRate = 44100.0;
    float depth = 1.0f;
    float rate = 1.0f;
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
    float setCutoff(float cutoff) override;

private:
    //juce::dsp::Oscillator<float> osc;  
};



class StepSequencer : public Modulator {
public:
    void setSteps(const std::array<float, 16>& values);
    void setNumActiveSteps(int n);
    float setCutoff(float cutoff) override {
        return cutoff;
    }

private:
    std::array<float, 16> stepValues{};
    int numActiveSteps = 16;
    int currentStep = 0;
    double phase = 0.0;
    double stepRate = 1.0; 

};

