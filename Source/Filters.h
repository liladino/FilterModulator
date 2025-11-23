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

class QuadFilter {
public:
    void setCoeffs(float _b0, float _b1, float _b2, float _a1, float _a2) {
        b0 = _b0; b1 = _b1; b2 = _b2; a1 = _a1; a2 = _a2;
    }
    float processSample(float x);
    void reset() {
        x1 = 0; x2 = 0; y1 = 0; y2 = 0;
    }

private:
    float b0, b1, b2, a1, a2;
    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
};
