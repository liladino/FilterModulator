/*
  ==============================================================================

    Filters.cpp
    Created: 23 Oct 2025 2:16:36pm
    Author:  BM

  ==============================================================================
*/

#include "Filters.h"

float QuadFilter::processSample(float x)
{
    float y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;

    return y;
}

const float PI = 3.14159265358f;

void Filter::processBlock(juce::AudioBuffer<float>& buffer) {
    int numChannels = buffer.getNumChannels();

    if (qfilters[0].size() != numChannels) {
        qfilters[0].resize(numChannels);
        qfilters[1].resize(numChannels);

        for (auto& f : qfilters[0]) f.reset();
        for (auto& f : qfilters[1]) f.reset();

        DBG("Filter resized for " << numChannels << " channels.");

        updateCoeffs(true);
    }
    else {
        updateCoeffs();
    }

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto* samples = buffer.getWritePointer(channel);
        auto& stage1 = qfilters[0][channel];
        auto& stage2 = qfilters[1][channel];

        for (int n = 0; n < buffer.getNumSamples(); ++n) {
            float s = samples[n];
            s = stage1.processSample(s);
            s = stage2.processSample(s);
            samples[n] = s;
        }
    }
}

LowPassFilter::LowPassFilter() {
    updateCoeffs();
}

void Filter::calcAndSetCoeffs() {
    //szurok egyutthato szamolas forras: https://www.ti.com/lit/an/slaa447/slaa447.pdf?utm_source=chatgpt.com&ts=1761396484063

    float w0 = 2.0f * PI * cutoff / sampleRate;
    float cosw0 = cos(w0);
    float sinw0 = sin(w0);

    float alpha1 = sinw0 / (2.0f * Q1 * resonance);
    float alpha2 = sinw0 / (2.0f * Q2 * resonance);
    setFilterCoeffs(cosw0, alpha1, 0);
    setFilterCoeffs(cosw0, alpha2, 1);
}

void LowPassFilter::setFilterCoeffs(float cosw0, float alpha, size_t index){
    float b0 = (1 - cosw0) / 2;
    float b1 = 1 - cosw0;
    float b2 = (1 - cosw0) / 2;
    float a0 = 1 + alpha;
    float a1 = -2 * cosw0;
    float a2 = 1 - alpha;

    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;

    for (auto& x : qfilters[index]) {
        x.setCoeffs(b0, b1, b2, a1, a2);
    }

    DBG("LowPassFilter::setFilterCoeffs() called");
}

HighPassFilter::HighPassFilter() {
    updateCoeffs();
}

void HighPassFilter::setFilterCoeffs(float cosw0, float alpha, size_t index) {
    float b0 = (1 + cosw0) / 2;
    float b1 = -(1 + cosw0);
    float b2 = (1 + cosw0) / 2;
    float a0 = 1 + alpha;
    float a1 = -2 * cosw0;
    float a2 = 1 - alpha;

    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;

    for (auto& x : qfilters[index]) {
        x.setCoeffs(b0, b1, b2, a1, a2);
    }

    DBG("HighPassFilter::setFilterCoeffs() called");
}
