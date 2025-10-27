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
    void reset() {
        x1 = 0; x2 = 0; y1 = 0; y2 = 0;
    }

private:
    float b0, b1, b2, a1, a2;
    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
};

class Filter {
public:
    virtual ~Filter() = default;
    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) { 
        this->sampleRate = sampleRate; 
        calcAndSetCoeffs();
    }
    virtual void processBlock(juce::AudioBuffer<float>& buffer);
    virtual void updateCoeffs(bool force = false) {
        constexpr float EPSILON = 0.05f;
        if (!force && 
            std::abs(cutoff - __cutoff) < EPSILON &&
            std::abs(resonance - __resonance) < EPSILON &&
            std::abs(sampleRate - __sampleRate) < EPSILON) {
            return;
        }
        DBG(cutoff << " " << __cutoff << " " << resonance << " " << __resonance);
        __cutoff = cutoff;
        __resonance = resonance;
        __sampleRate = sampleRate;

        calcAndSetCoeffs();
    }

    void setCutoff(float freq) { cutoff = freq; }
    void setResonance(float q) { resonance = q; }
    float getCutoff() { return cutoff; }
    float getResonance() { return resonance; }

protected:
    // Butterworth 4edfoku szuro alapertekek
    // https://www.earlevel.com/main/2016/09/29/cascading-filters/
    const float Q1 = 0.541196f;
    const float Q2 = 1.306563f;

    float cutoff = 1000.0f;
    float resonance = 0.707f;
    
    float __cutoff = 1000.0f;
    float __resonance = 0.707f;

    double sampleRate = 44100.0;
    double __sampleRate = 44100.0;

    std::array<std::vector<QuadFilter>, 2> qfilters;

    virtual void calcAndSetCoeffs();
    virtual void setFilterCoeffs(float cosw0, float alpha, size_t index) = 0;
};

class LowPassFilter : public Filter {
public:
    LowPassFilter();
    void setFilterCoeffs(float cosw0, float alpha, size_t index) override;
private:
};


class HighPassFilter : public Filter {
public:
    HighPassFilter();
    void setFilterCoeffs(float cosw0, float alpha, size_t index) override;
private:
};
