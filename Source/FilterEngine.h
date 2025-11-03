/*
  ==============================================================================

    FilterEngine.h
    Created: 23 Oct 2025 5:07:24pm
    Author:  BM

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "Modulator.h"
#include "Filters.h"


class FilterEngine {
public:
    FilterEngine()
        : LP(std::make_unique<LowPassFilter>()), HP(std::make_unique<HighPassFilter>()),
          seq(std::make_unique<StepSequencer>())
    {
        cutoff = 500;
        resonance = 1;
        filter = LP.get();
        modulator = nullptr;
    }

    //void bindCutoff(std::atomic<float>* param) { cutoff = param; }
    //void bindRes(std::atomic<float>* param) { resonance = param; }

    void prepareToPlay(double sampleRate, int samplesPerBlock);

    void processBlock(juce::AudioBuffer<float>& buffer);

    enum class FilterMode { LowPass, HighPass };
    enum class ModulatorMode { Off, LFO, Seq};

    void setFilterMode(FilterMode newMode);

    void setModulator(ModulatorMode newMode);

    void setResonance(float resonance);

    void setCutoff(float cutoff);

    void setSequencerStep(int index, float cutoff);

    void setSequencerNum(int value);
private:
    float cutoff, resonance; 

    Filter* filter = nullptr;
    std::unique_ptr<LowPassFilter> LP;
    std::unique_ptr<HighPassFilter> HP;
    
    Modulator* modulator;
    //std::unique_ptr<Oscillator> lfo;
    std::unique_ptr<StepSequencer> seq;

    FilterMode fmode = FilterMode::LowPass;
    ModulatorMode mmode = ModulatorMode::Off;
};