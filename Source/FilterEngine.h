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
#include "BWFilter.h"
#include "Filters.h"

class FilterEngine {
public:
    FilterEngine()
        : LP(std::make_unique<LowPassFilter>()), HP(std::make_unique<HighPassFilter>()),
          seq(std::make_unique<StepSequencer>()), lfo(std::make_unique<Oscillator>())
    {
        _cutoff = 500;
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
    void setModWaveType(WaveGenerator::WaveType newMode);
    void setResonance(float resonance);
    void setRate(float rateHz);
    void setRate(); //set the last used rate (amd switch back from bpm mode)
    void setCutoff(float cutoff);
    void setSequencerStep(int index, float cutoff);
    void setSequencerNum(int value);
    void setLFOwidth(float value);
    void setBPMnoteLength(float noteLength);
    void syncToBPM(float bpm);
    void syncToBPM(); //set the last used bpm 
private:
    float lastRate = 1;
    bool BPMsynced = false;
    float lastBPM = 60;
    float noteLength = 1;
    void setCutoffInFiltersOnly(float cutoff);
    float _cutoff; 

    Filter* filter = nullptr;
    std::unique_ptr<LowPassFilter> LP;
    std::unique_ptr<HighPassFilter> HP;
    
    Modulator* modulator;
    std::unique_ptr<Oscillator> lfo;
    std::unique_ptr<StepSequencer> seq;

    FilterMode fmode = FilterMode::LowPass;
    ModulatorMode mmode = ModulatorMode::Off;
};