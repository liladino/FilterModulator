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
    virtual void prepareToPlay(double sampleRate) { this->sampleRate = sampleRate; }
    virtual float setCutoff(float cutoff) = 0;  //in [0,1]
    void setRate(float rateHz);
    void syncToBPM(double bpm);

protected:
    double sampleRate = 44100.0;
    float depth = 1.0f;
    float rate = 1.0f;
};



class LFO : public Modulator{
public:
    enum WaveType { Sine, Triangle, Square };
    void setWaveType(WaveType type);
    void setDepth(float depthAmount);
    float setCutoff(float cutoff) override;

private:
    //juce::dsp::Oscillator<float> osc;  
};



class StepSequencer : public Modulator {
public:
    void setSteps(const std::array<float, 16>& values);
    void setNumActiveSteps(int n);
    float setCutoff(float cutoff) override;

private:
    std::array<float, 16> stepValues{};
    int numActiveSteps = 16;
    int currentStep = 0;
    double phase = 0.0;
    double stepRate = 1.0; 
};