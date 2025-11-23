/*
  ==============================================================================

    MoogFilter.cpp
    Created: 17 Nov 2025 4:57:55pm
    Author:  BM

  ==============================================================================
*/

#include "MoogFilter.h"

void MoogFilter::prepareToPlay(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    setCutoff(500.f);
    setResonance(1.f);
}
void MoogFilter::processBlock(juce::AudioBuffer<float>& buffer, int numProcessed, const int samplesThisTime) {
    int numChannels = buffer.getNumChannels();

    if (poles[0].size() != numChannels) {
        for (int i = 0; i < 4; i++) {
            poles[i].resize(numChannels);
            for (auto& f : poles[i]) f.reset();
        }

        DBG("Filter resized for " << numChannels << " channels.");
    }

    for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
        auto* samples = buffer.getWritePointer(channel);

        for (int n = numProcessed; n < numProcessed + samplesThisTime; n++) {
            std::array<float, 5> arr;
            arr[0] = samples[n] - resonance * std::tanh(0.5f * samples[n] + poles[3][channel].y); // feedback
            arr[1] = poles[0][channel].processSample(arr[0], g);
            arr[2] = poles[1][channel].processSample(arr[1], g);
            arr[3] = poles[2][channel].processSample(arr[2], g);
            arr[4] = poles[3][channel].processSample(arr[3], g);

            float acc = 0;
            for (int i = 0; i < 5; i++) {
                acc += arr[i] * ABCDE[i];
            }

            samples[n] = acc;
        }
    }
}

void MoogFilter::setCutoff(float freq) {
    cutoff = freq;
    wc = 2 * PI * cutoff / sampleRate;
    g = 0.9892f * wc - 0.4342f * wc * wc + 0.1381f * wc * wc * wc - 0.0202f * wc * wc * wc * wc;
    setResonance(lastq);
    /*if (_type != MoogType::MoogLP) {
        for (int i = 0; i < 4; i++) {
            for (auto& x : poles[i]) x.reset();
        }
    }*/

}
void MoogFilter::setResonance(float q) {
    /* A Moog filter akkor lapos, ha a resonancia 0,
     * a ui-ban 0.5 es 5 kozott allitunk erteket, de 1.3
     felett a Moog mar kezd szetesni. */
    lastq = q;
    resonance = ((q - 0.45f) / 5.f * 1.6f) * (1.0029f + 0.0526f * wc - 0.0926f * wc * wc + 0.0218f * wc * wc * wc);
    //DBG("resonance set to " << q << ' ' << lastq);
}