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

    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        LP->prepareToPlay(sampleRate, samplesPerBlock);
        HP->prepareToPlay(sampleRate, samplesPerBlock);
        seq->prepareToPlay(sampleRate, samplesPerBlock);
        //lfo->prepareToPlay(sampleRate, samplesPerBlock);
    }

    void processBlock(juce::AudioBuffer<float>& buffer){
        if (modulator != nullptr) {
            setCutoff(modulator->setCutoff(cutoff));
        }
        
        //filter->setCutoff(cutoff == nullptr ? 1000.0f : cutoff->load());
        //filter->setResonance(resonance == nullptr ? 1.0f : resonance->load());

        filter->processBlock(buffer);
    }

    enum class FilterMode { LowPass, HighPass };
    enum class ModulatorMode { Off, LFO, Seq};

    void setFilterMode(FilterMode newMode) { 
        fmode = newMode; 
        if (fmode == FilterMode::LowPass) {
            filter = LP.get();
        }
        else {
            filter = HP.get();
        }
    }

    void setModulator(ModulatorMode newMode) {
        mmode = newMode; 
        switch (mmode) {
            case ModulatorMode::Off:
                modulator = nullptr;
                break;
            case ModulatorMode::Seq:
                modulator = seq.get();
                break;
            case ModulatorMode::LFO:
                modulator = nullptr;
                break;
        }
    }

    void setResonance(float resonance) {
        LP->setResonance(resonance);
        HP->setResonance(resonance);
    }

    void setCutoff(float cutoff) {
        LP->setCutoff(cutoff);
        HP->setCutoff(cutoff);
    }

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