/*
  ==============================================================================

    FilterEngine.cpp
    Created: 23 Oct 2025 5:07:24pm
    Author:  BM

  ==============================================================================
*/

#include "FilterEngine.h"

void FilterEngine::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BWLP->prepareToPlay(sampleRate, samplesPerBlock);
    BWHP->prepareToPlay(sampleRate, samplesPerBlock);
    BWN->prepareToPlay(sampleRate, samplesPerBlock);
    MLP->prepareToPlay(sampleRate, samplesPerBlock);
    MHP->prepareToPlay(sampleRate, samplesPerBlock);
    MBP->prepareToPlay(sampleRate, samplesPerBlock);
    seq->prepareToPlay(sampleRate, samplesPerBlock);
    lfo->prepareToPlay(sampleRate, samplesPerBlock);
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
            setCutoffInFiltersOnly(modulator->getModulatedCutoff(_cutoff, samplesThisTime));
        }

        filter->processBlock(buffer, numProcessed, samplesThisTime);

        numProcessed += samplesThisTime;
    }
}

void FilterEngine::setFilterMode(FilterMode newMode) {
    fmode = newMode;
    switch (fmode) {
    default:                       filter = BWLP.get(); break;
    case FilterMode::BWHighPass:   filter = BWHP.get(); break;
    case FilterMode::BWNotch:      filter = BWN.get(); break;
    case FilterMode::MoogLowPass:  filter = MLP.get(); break;
    case FilterMode::MoogHighPass: filter = MHP.get(); break;
    case FilterMode::MoogBandPass: filter = MBP.get(); break;
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
        modulator = lfo.get();
        break;
    }
}

void FilterEngine::setModWaveType(WaveGenerator::WaveType newMode) {
    lfo->setWaveType(newMode);
   // DBG("WaveType " << newMode);
}

void FilterEngine::setResonance(float resonance) {
    BWLP->setResonance(resonance);
    BWHP->setResonance(resonance);
    BWN->setResonance(resonance);
    MLP->setResonance(resonance);
    MHP->setResonance(resonance);
    MBP->setResonance(resonance);
}

void FilterEngine::setRate(float rateHz) {
    BPMsynced = false;
    lfo->setRate(rateHz);
    seq->setRate(rateHz);
    lastRate = rateHz;
}

void FilterEngine::setRate() {
    BPMsynced = false;
    lfo->setRate(lastRate);
    seq->setRate(lastRate);
}

void FilterEngine::setCutoffInFiltersOnly(float cutoff) {
    BWLP->setCutoff(cutoff);
    BWHP->setCutoff(cutoff);
    BWN->setCutoff(cutoff);
    MLP->setCutoff(cutoff);
    MHP->setCutoff(cutoff);
    MBP->setCutoff(cutoff);

    DBG(cutoff);
}

void FilterEngine::setCutoff(float cutoff) {
    _cutoff = cutoff;
    setCutoffInFiltersOnly(cutoff);
}

void FilterEngine::setSequencerStep(int index, float cutoff) {
    seq->setFilter(index, cutoff);
}

void FilterEngine::setSequencerNum(int value) {
    seq->setNumActiveSteps(value);
}

void FilterEngine::setLFOwidth(float value) {
    //set the full amplitude
    lfo->setDepth(value / 2.f);
}

void FilterEngine::syncToBPM(float bpm) {
    BPMsynced = true;
    lastBPM = bpm;
    lfo->syncToBPM(bpm * noteLength);
    seq->syncToBPM(bpm * noteLength);

    DBG("BPM " << bpm);
}

void FilterEngine::syncToBPM() {
    syncToBPM(lastBPM);
}

void FilterEngine::setBPMnoteLength(float noteLength) {
    this->noteLength = noteLength;
    if (BPMsynced) {
        DBG("tempo: " << lastBPM * noteLength);
        syncToBPM();
    }
}