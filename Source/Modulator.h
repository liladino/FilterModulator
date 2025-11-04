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
    //virtual void syncToBPM(double bpm) = 0;

protected:
    double sampleRate = 44100.f;
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
private:
    void updateHalfCycle() {
        samplesPerHalfCycle = static_cast<int>(0.5f * sampleRate / rate);
    }

    std::array<float, 16> stepValues;
    unsigned long long sampleCount = 0;
    int numActiveSteps = 2;
    int currentStep = 0;
    float rate = 1.f;
    int samplesPerHalfCycle;
};


class WaveGenerator {
public:
    const float PI = 3.14159265358f;
    enum WaveType { Sin, Square, Triangle, Saw };

    void prepareToPlay(float sampleRate) {
        this->sampleRate = sampleRate;
        updateIncrement();
    }

    void setFrequency(float newFreq) {
        frequency = newFreq;
        updateIncrement();
    }

    float getNextValue(int samplesPerBlock) {
        float value = std::sin(2.0f * PI * phase);

        phase += samplesPerBlock * phaseIncrement;
        if (phase >= 1.0f) {
            phase = phase - floor(phase);
        }
        return value;
    }

    void setWaveType(WaveType wt) {
        type = wt;
    }

private:
    void updateIncrement() {
        phaseIncrement = static_cast<double>(frequency) / static_cast<double>(sampleRate);
    }
    WaveType type = WaveType::Sin;
    float sampleRate = 44100.0;
    float frequency = 1.0f;
    double phase = 0;
    float phaseIncrement = 0.0f;
};


class Oscillator : public Modulator {
public:
    Oscillator() {
        depth = semitone;
        wgenerator.prepareToPlay(sampleRate);
    }
    void prepareToPlay(float sampleRate, int samplesPerBlock) override {
        this->sampleRate = sampleRate;
        wgenerator.prepareToPlay(sampleRate);
        wgenerator.setFrequency(rate);
    }

    void setWaveType(WaveGenerator::WaveType type){}
    
    void setDepth(float semitoneDepth) {
        //depth = (2^(1/12))^semitoneDepth
        depth = pow(semitone, semitoneDepth);
        DBG("depth = " << semitoneDepth << " multiplier: " << depth);
    }

    void setRate(float rateHz) { 
        rate = rateHz;
        wgenerator.setFrequency(rate);
    }

    float getModulatedCutoff(float cutoff, int samplesPerBlock) override {
        return std::clamp(cutoff * pow(depth, wgenerator.getNextValue(samplesPerBlock)), 21.f, 19990.f);
    }

private:
    const float semitone = pow(2.f, 0.0833333333f); //2^(1/12)

    WaveGenerator wgenerator;
    float depth = 1.0f;
};

