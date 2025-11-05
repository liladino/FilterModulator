/*
  ==============================================================================

    Filters.h
    Created: 23 Oct 2025 2:16:36pm
    Author:  BM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class Filter {
public:
    virtual ~Filter() = default;
    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) = 0;
    virtual void processBlock(juce::AudioBuffer<float>& buffer, int numProcessed, const int samplesThisTime) = 0;
    virtual void setCutoff(float freq) = 0;
    virtual void setResonance(float q) = 0;
    virtual float getCutoff() = 0;
    virtual float getResonance() = 0;
};
