/*
  ==============================================================================

    FilterEngine.cpp
    Created: 23 Oct 2025 5:07:24pm
    Author:  BM

  ==============================================================================
*/

#include "FilterEngine.h"

void FilterEngine::prepareToPlay(double sampleRate, int samplesPerBlock) {
    LP->prepareToPlay(sampleRate, samplesPerBlock);
    HP->prepareToPlay(sampleRate, samplesPerBlock);
    seq->prepareToPlay(sampleRate, samplesPerBlock);
    //lfo->prepareToPlay(sampleRate, samplesPerBlock);
}

void FilterEngine::processBlock(juce::AudioBuffer<float>& buffer) {
    /*
     * Ha a buffer tul nagy, a modulator szamolasa elcsuszik:
     * mivel mindig blokkmeretet ad hozza a sampleCount-jahoz,
     * nagy blokkemerettel atlapolodhat a modulacio. (foleg
     * rendering kozben lehet tobb ezer)
     * Ezert 64 sample-os subblokkokat processelek, igy mindig
     * allando, minimalis hibaval dolgozom.
     */
    const int totalSamples = buffer.getNumSamples();
    const int subBlockSize = 64;
    int numProcessed = 0;

    while (numProcessed < totalSamples) {
        const int samplesThisTime = std::min(subBlockSize, totalSamples - numProcessed);

        if (modulator != nullptr) {
            setCutoff(modulator->getModulatedCutoff(cutoff, samplesThisTime));
        }

        filter->processBlock(buffer, numProcessed, samplesThisTime);

        numProcessed += samplesThisTime;
    }
}

void FilterEngine::setFilterMode(FilterMode newMode) {
    fmode = newMode;
    if (fmode == FilterMode::LowPass) {
        filter = LP.get();
    }
    else {
        filter = HP.get();
    }
}


void FilterEngine::setModulator(ModulatorMode newMode) {
    mmode = newMode;
    switch (mmode) {
    case ModulatorMode::Off:
        modulator = nullptr;
        break;
    case ModulatorMode::Seq:
        modulator = seq.get();
        break;
    case ModulatorMode::LFO:
        modulator = nullptr;
        break;
    }
}

void FilterEngine::setResonance(float resonance) {
    LP->setResonance(resonance);
    HP->setResonance(resonance);
}

void FilterEngine::setCutoff(float cutoff) {
    LP->setCutoff(cutoff);
    HP->setCutoff(cutoff);
}

void FilterEngine::setSequencerStep(int index, float cutoff) {
    seq->setFilter(index, cutoff);
}

void FilterEngine::setSequencerNum(int value) {
    seq->setNumActiveSteps(value);
}