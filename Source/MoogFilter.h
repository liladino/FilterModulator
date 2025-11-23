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
    enum class MoogType { MoogLP, MoogHP, MoogBP };
        
	MoogFilter(MoogType type = MoogType::MoogLP) {
		switch (type){
		case MoogType::MoogLP: 	
			ABCDE = {0, 0, 0, 0, 1};
			break;
		case MoogType::MoogHP:
			ABCDE = {1, -4, 6, -4, 1};
			break;
		case MoogType::MoogBP:
			ABCDE = {0, 0, 4, -8, 4};
			break;
		}
	}
		
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, int numProcessed, const int samplesThisTime) override;

    void setCutoff(float freq) override;
    void setResonance(float q) override;

    float getCutoff() override { return cutoff; }
    float getResonance() override { return resonance; }

private:
    float lastq = 0.5f;

    const float PI = 3.14159265358f;

    double sampleRate = 48000.f;
    float cutoff = 500.f;

    float wc = 2 * PI * cutoff / sampleRate;
    float g = 0.9892f * wc - 0.4342f * wc * wc + 0.1381f * wc * wc * wc - 0.0202f * wc * wc * wc * wc;
    
    float resonance = 0.f;
    
    std::array<float, 5> ABCDE = {0, 0, 0, 0, 1};

    std::array<std::vector<MoogPole>, 4> poles;
};
