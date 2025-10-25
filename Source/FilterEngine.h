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
        : LP(std::make_unique<LowPassFilter>()), HP(std::make_unique<HighPassFilter>())
    {
        filter = LP.get();
    }

    void bindCutoff(std::atomic<float>* param) { cutoff = param; }
    void bindRes(std::atomic<float>* param) { resonance = param; }
    void bindHighpass(std::atomic<float>* param) { resonance = param; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        filter->prepareToPlay(sampleRate, samplesPerBlock);
        //modulator->prepare(sampleRate);
    }

    void processBlock(juce::AudioBuffer<float>& buffer);


    enum class FilterMode { LowPass, HighPass };

    void changeFilterMode() { (fmode == FilterMode::LowPass ? setFilterMode(FilterMode::LowPass) : setFilterMode(FilterMode::HighPass)); }

    void setFilterMode(FilterMode newMode) { 
        fmode = newMode; 
        if (fmode == FilterMode::LowPass) {
            filter = LP.get();
        }
        else {
            filter = HP.get();
        }
    }

private:
    std::atomic<float>* cutoff = nullptr;
    std::atomic<float>* resonance = nullptr;

    Filter* filter = nullptr;
    std::unique_ptr<LowPassFilter> LP;
    std::unique_ptr<HighPassFilter> HP;
    
    //std::unique_ptr<Modulator> modulator;

    FilterMode fmode = FilterMode::LowPass;
};