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
    virtual void prepareToPlay(float sampleRate, int samplesPerBlock) = 0;
    virtual float getModulatedCutoff(float cutoff, int samplesPerBlock) = 0;  
    virtual void setRate(float rateHz) = 0;
    virtual void setPhase(double phase) = 0;
    virtual void syncToBPM(float bpm) {
        if (bpm == 0) { bpm = 60; }
        setRate(bpm / 60.f);
    }
protected:
    double sampleRate = 48000.f;
    float rate = 1.0f; //hz
};


class StepSequencer : public Modulator {
public:
    StepSequencer();
    void prepareToPlay(float sampleRate, int samplesPerBlock);
    float getModulatedCutoff(float cutoff, int samplesPerBlock) override;
    void setRate(float rateHz) override;
    void setFilter(int index, float cutoff);
    void setNumActiveSteps(int n);
    void setPhase(double phase) override;
private:
    void updateCycle() {
        samplesPerCycle = static_cast<int>(sampleRate / rate);
    }

    std::array<float, 16> stepValues;
    unsigned long long sampleCount = 0;
    int numActiveSteps = 2;
    int currentStep = 0;
    int samplesPerCycle;
};


class WaveGenerator {
public:
    const float PI = 3.14159265358f;
    enum class WaveType { Sin, Square, Saw, MinusSaw };

    void prepareToPlay(float sampleRate);
    void setFrequency(float newFreq);
    float getNextValue(int samplesPerBlock);
    void setWaveType(WaveType wt);
    void setPhase(double _phase);
private:
    void updateIncrement();
    WaveType type = WaveType::Sin;
    float sampleRate = 48000.0;
    float frequency = 1.0f;
    double phase = 0;
    float phaseIncrement = 0.0f;
};


class Oscillator : public Modulator {
public:
    Oscillator();
    void prepareToPlay(float sampleRate, int samplesPerBlock) override;
    void setWaveType(WaveGenerator::WaveType type);    
    void setDepth(float semitoneDepth);
    void setRate(float rateHz) override;
    float getModulatedCutoff(float cutoff, int samplesPerBlock) override;
    void setPhase(double phase) override;
private:
    const float semitone = pow(2.f, 0.0833333333f); //2^(1/12)

    WaveGenerator wgenerator;
    float depth = 1.0f;
};

