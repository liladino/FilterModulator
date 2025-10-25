/*
  ==============================================================================

    Filters.h
    Created: 23 Oct 2025 2:16:36pm
    Author:  BM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class QuadFilter {
public:
    void setCoeffs(float _b0, float _b1, float _b2, float _a1, float _a2) {
        b0 = _b0; b1 = _b1; b2 = _b2; a1 = _a1; a2 = _a2;
    }
    float processSample(float x);
private:
    float b0, b1, b2, a1, a2;
    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
};

class Filter {
public:
    virtual ~Filter() = default;
    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) { this->sampleRate = sampleRate; calcCoeffs(); }
    virtual void processBlock(juce::AudioBuffer<float>& buffer) = 0;
    virtual void calcCoeffs() = 0;
    virtual void updateCoeffs() {
        constexpr float EPSILON = 0.1f;
        if (std::abs(cutoff - __cutoff) < EPSILON &&
            std::abs(resonance - __resonance) < EPSILON) {
            return;
        }
        DBG(cutoff << " " << __cutoff << " " << resonance << " " << __resonance);
        __cutoff = cutoff;
        __resonance = resonance;
        calcCoeffs();
    }

    void setCutoff(float freq) { cutoff = freq; }
    void setResonance(float q) { resonance = q; }
    float getCutoff() { return cutoff; }
    float getResonance() { return resonance; }

protected:
    float cutoff = 1000.0f;
    float resonance = 0.707f;
    
    float __cutoff = 1000.0f;
    float __resonance = 0.707f;

    double sampleRate = 44100.0;

    QuadFilter qfilter1;
    QuadFilter qfilter2;
};

class LowPassFilter : public Filter {
public:
    LowPassFilter();
    
    //void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;
    void calcCoeffs() override;
private:
};


class HighPassFilter : public Filter {
public:
    HighPassFilter();
    
    //void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;
    void calcCoeffs() override;
private:
};
