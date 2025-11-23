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
    virtual void processBlock(juce::AudioBuffer<float>& buffer, int numProcessed, const int samplesThisTime) = 0;
    virtual void updateCoeffs(const int samplesThisTime, bool force = false);
    virtual void setCutoff(float freq) override {
        cutoff = freq;
        cutoffSmoother.setTarget(freq);
    }
    virtual void setResonance(float q) override { resonance = q; }
    virtual float getCutoff() override { return cutoff; }
    virtual float getResonance() override { return resonance; }

protected:
    ParameterSmoother cutoffSmoother;

    float cutoff = 500.0f;
    float resonance = 1.f;
    double sampleRate = 48000.f;

    // Ertek valtozas detektalashoz
    float __cutoff = 500.0f;
    float __resonance = 1.f;
    double __sampleRate = 48000.f;

    virtual void calcAndSetCoeffs() = 0;
    virtual void setFilterCoeffs(float cosw0, float alpha, std::vector<QuadFilter>& filterList) = 0;
};

class BWNotch : public BWFilter {
public:
    BWNotch();
    void processBlock(juce::AudioBuffer<float>& buffer, int numProcessed, const int samplesThisTime);
protected:
    ParameterSmoother cutoffSmoother;
    std::vector<QuadFilter> filter;

    void calcAndSetCoeffs();
    void setFilterCoeffs(float cosw0, float alpha, std::vector<QuadFilter>& filterList);
};

class BW4LPHP : public BWFilter {
public:
    virtual ~BW4LPHP() = default;
    virtual void processBlock(juce::AudioBuffer<float>& buffer, int numProcessed, const int samplesThisTime);
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

    std::array<std::vector<QuadFilter>, 2> qfilters;

    virtual void calcAndSetCoeffs();
    virtual void setFilterCoeffs(float cosw0, float alpha, std::vector<QuadFilter>& filterList) = 0;
};

class BWLowPFilter : public BW4LPHP {
public:
    BWLowPFilter();
    void setFilterCoeffs(float cosw0, float alpha, std::vector<QuadFilter>& filterList) override;
private:
};


class BWHighPFilter : public BW4LPHP {
public:
    BWHighPFilter();
    void setFilterCoeffs(float cosw0, float alpha, std::vector<QuadFilter>& filterList) override;
private:
};
