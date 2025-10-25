/*
  ==============================================================================

    FilterEngine.cpp
    Created: 23 Oct 2025 5:07:24pm
    Author:  pampa

  ==============================================================================
*/

#include "FilterEngine.h"


void FilterEngine::processBlock(juce::AudioBuffer<float>& buffer) {
    //auto modValue = modulator->getNextValue();
    //float modcutoff = cutoff * modValue;

    filter->setCutoff(cutoff == nullptr ? 1000.0f : cutoff->load());
    filter->setResonance(resonance == nullptr ? 1.0f : resonance->load());

    //DBG("cutoff param: " << (cutoff == nullptr ? -999.0f : cutoff->load()));

    filter->processBlock(buffer);
}