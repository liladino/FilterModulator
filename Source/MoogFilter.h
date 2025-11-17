/*
  ==============================================================================

    MoogFilter.h
    Created: 17 Nov 2025 4:57:55pm
    Author:  BM

  ==============================================================================
*/

#pragma once

#include "Filters.h"

struct MoogPole {
    float x1;  // elozo input
    float y1;  // elozo output
    float y;
    
    float processSample(float x, float g) {
        y = g * (x + 0.3f * x1) + (1.0f - g) * y1;
        x1 = x;
        y1 = y;
        return y;
    }

    void reset() {
        x1 = y1 = y = 0;
    }
};

class MoogFilter : public Filter {
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        this->sampleRate = sampleRate;
        setCutoff(500.f);
        setResonance(1.f);
    }
    void processBlock(juce::AudioBuffer<float>& buffer, int numProcessed, const int samplesThisTime) override {
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
                float s = samples[n] - resonance * std::tanh(/* szorzo kerulhet ide */ poles[3][channel].y); // feedback
                for (int i = 0; i < 4; i++) {
                    s = poles[i][channel].processSample(s, g);
                }

                samples[n] = s;
            }
        }
    }

    void setCutoff(float freq) override {
        cutoff = freq;
        wc = 2 * PI * cutoff / sampleRate;
        g = 0.9892f * wc - 0.4342f * wc * wc + 0.1381f * wc * wc * wc - 0.0202f * wc * wc * wc * wc;
    }
    void setResonance(float q) override {
        /* A Moog filter akkor lapos, ha a resonancia 0, 
         * a ui-ban 0.5 es 5 kozott allitunk erteket */
        resonance = (q - 0.45f) * (1.0029f + 0.0526f * wc - 0.0926f * wc * wc + 0.0218 * wc * wc * wc);
    }

    float getCutoff() override { return cutoff; }
    float getResonance() override { return resonance; }

private:
    const float PI = 3.14159265358f;

    double sampleRate = 44100.0;
    float cutoff = 500.f;

    float wc = 2 * PI * cutoff / sampleRate;
    float g = 0.9892f * wc - 0.4342f * wc * wc + 0.1381f * wc * wc * wc - 0.0202f * wc * wc * wc * wc;
    
    float resonance = 0.f;

    std::array<std::vector<MoogPole>, 4> poles;
};
