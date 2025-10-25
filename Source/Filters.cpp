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
    updateCoeffs();

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto* samples = buffer.getWritePointer(channel);
        for (int n = 0; n < buffer.getNumSamples(); ++n) {
            float s = samples[n];
            s = qfilter1.processSample(s);
            s = qfilter2.processSample(s);
            samples[n] = s;
        }
    }
}

LowPassFilter::LowPassFilter() {
    updateCoeffs();
}

void LowPassFilter::calcCoeffs() {
    //forras: https://www.ti.com/lit/an/slaa447/slaa447.pdf?utm_source=chatgpt.com&ts=1761396484063&ref_url=https%253A%252F%252Fchatgpt.com%252F

    float w0 = 2.0f * PI * cutoff / sampleRate;
    float cosw0 = cos(w0);
    float sinw0 = sin(w0);

    //a rezonancia gyoke, mert kaszkadositok 2 masodfoku szurot
    float alpha = sinw0 / (2.0f * sqrt(resonance));

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
    
    qfilter1.setCoeffs(b0, b1, b2, a1, a2);
    qfilter2.setCoeffs(b0, b1, b2, a1, a2);

    DBG("LowPassFilter::calcCoeffs() called");
}

HighPassFilter::HighPassFilter() {
    //forras: https://www.ti.com/lit/an/slaa447/slaa447.pdf?utm_source=chatgpt.com&ts=1761396484063&ref_url=https%253A%252F%252Fchatgpt.com%252F

    float w0 = 2.0f * PI * cutoff / sampleRate;
    float cosw0 = cos(w0);
    float sinw0 = sin(w0);

    //a rezonancia gyoke, mert kaszkadositok 2 masodfoku szurot
    float alpha = sinw0 / (2.0f * sqrt(resonance));

    float b0 = (1 + cosw0) / 2;
    float b1 = 1 + cosw0;
    float b2 = (1 + cosw0) / 2;
    float a0 = 1 + alpha;
    float a1 = -2 * cosw0;
    float a2 = 1 - alpha;

    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;

    qfilter1.setCoeffs(b0, b1, b2, a1, a2);
    qfilter2.setCoeffs(b0, b1, b2, a1, a2);

    DBG("LowPassFilter::calcCoeffs() called");
    updateCoeffs();
}


void HighPassFilter::calcCoeffs() {

}
