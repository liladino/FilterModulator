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
#include "MoogFilter.h"
#include "Filters.h"

class FilterEngine {
public:
    FilterEngine()
        : BWLP(std::make_unique<LowPassFilter>()), BWHP(std::make_unique<HighPassFilter>()),
          MLP(std::make_unique<MoogFilter>()),
          MHP(std::make_unique<MoogFilter>(MoogFilter::MoogType::MoogHP)),
          MBP(std::make_unique<MoogFilter>(MoogFilter::MoogType::MoogBP)),
          seq(std::make_unique<StepSequencer>()), lfo(std::make_unique<Oscillator>())
    {
        _cutoff = 500;
        filter = BWLP.get();
        modulator = nullptr;
    }

    //void bindCutoff(std::atomic<float>* param) { cutoff = param; }
    //void bindRes(std::atomic<float>* param) { resonance = param; }

    void prepareToPlay(double sampleRate, int samplesPerBlock);

    void processBlock(juce::AudioBuffer<float>& buffer);

    enum class FilterMode { BWLowPass, BWHighPass, MoogLowPass, MoogHighPass, MoogBandPass };
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
    std::unique_ptr<LowPassFilter> BWLP;
    std::unique_ptr<HighPassFilter> BWHP;
    std::unique_ptr<MoogFilter> MLP;
    std::unique_ptr<MoogFilter> MHP;
    std::unique_ptr<MoogFilter> MBP;
    
    Modulator* modulator;
    std::unique_ptr<Oscillator> lfo;
    std::unique_ptr<StepSequencer> seq;

    FilterMode fmode = FilterMode::BWLowPass;
    ModulatorMode mmode = ModulatorMode::Off;
};