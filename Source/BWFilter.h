/*
  ==============================================================================

    BWFilter.h
    Created: 16 Nov 2025 8:49:17pm
    Author:  BM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Filters.h"

struct ParameterSmoother {
    float value = 0.0f;
    float target = 0.0f;
    float coeff = 0.0f; // smoothing coefficient, 0..1

    void reset(double sampleRate, float smoothingTimeSec) {
        coeff = std::exp(-1.0f / (smoothingTimeSec * (float)sampleRate));
    }

    void setTarget(float t) { target = t; }

    float getNextValue(int samples) {
        float stepCoeff = std::pow(coeff, (float)samples);
        value = target + stepCoeff * (value - target);
        return value;
    }

    void setCurrentAndTargetValue(float v) {
        value = target = v;
    }
};

class BWFilter : public Filter {
public:
    virtual ~BWFilter() = default;
    virtual void prepareToPlay(double sampleRate, int samplesPerBlock) {
        cutoffSmoother.reset(sampleRate, 0.01f);
        cutoffSmoother.setCurrentAndTargetValue(cutoff);

        this->sampleRate = sampleRate;
        calcAndSetCoeffs();
    }
    virtual void processBlock(juce::AudioBuffer<float>& buffer, int numProcessed, const int samplesThisTime);
    virtual void updateCoeffs(const int samplesThisTime, bool force = false);
    virtual void setCutoff(float freq) override {
        cutoff = freq;
        cutoffSmoother.setTarget(freq);
    }
    virtual void setResonance(float q) override { resonance = q; }
    virtual float getCutoff() override { return cutoff; }
    virtual float getResonance() override { return resonance; }

protected:
    // Butterworth 4edfoku szuro alapertekek
    const float Q1 = 0.541196f;
    const float Q2 = 1.306563f;

    /*
    Rezonancia:
        egy skalar szorzo, ami beallitja, hogy a ket biquad szuro
        Q1 es Q2 ertekeit milyen s-sel szorozzuk (ref. Matlab).

        Q1 = 0.5411961
        Q2 = 1.3065630

        (4edfoku kaszkados Butterwoth szuro ertekei)
        https://www.earlevel.com/main/2016/09/29/cascading-filters/
    */

    ParameterSmoother cutoffSmoother;

    float cutoff = 500.0f;
    float resonance = 1.f;

    float __cutoff = 500.0f;
    float __resonance = 1.f;

    double sampleRate = 44100.0;
    double __sampleRate = 44100.0;

    std::array<std::vector<QuadFilter>, 2> qfilters;

    virtual void calcAndSetCoeffs();
    virtual void setFilterCoeffs(float cosw0, float alpha, size_t index) = 0;
};

class LowPassFilter : public BWFilter {
public:
    LowPassFilter();
    void setFilterCoeffs(float cosw0, float alpha, size_t index) override;
private:
};


class HighPassFilter : public BWFilter {
public:
    HighPassFilter();
    void setFilterCoeffs(float cosw0, float alpha, size_t index) override;
private:
};
