/*
  ==============================================================================

    Modulator.cpp
    Created: 23 Oct 2025 2:17:00pm
    Author:  BM

  ==============================================================================
*/

#include "Modulator.h"

StepSequencer::StepSequencer() {
    for (int i = 0; i < 16; i++) stepValues[i] = 500.f;

    samplesPerHalfCycle = static_cast<int>(0.5f * sampleRate / rate / numActiveSteps);
}

void StepSequencer::prepareToPlay(float sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    setRate(rate); //re-calculate samplesPerHalfCycle if sampleRate may have changed
}

float StepSequencer::getModulatedCutoff(float cutoff, int samplesPerBlock) {
    sampleCount += samplesPerBlock;

    if (sampleCount >= samplesPerHalfCycle) {
        currentStep = (currentStep + 1) % numActiveSteps;
        sampleCount = 0;

        DBG("Step sequencer stepped " << currentStep << ' ' << samplesPerHalfCycle);
    }

    return stepValues[currentStep];
}

void StepSequencer::setRate(float rateHz) {
    rate = rateHz;
    updateHalfCycle();
}

void StepSequencer::setFilter(int index, float cutoff) {
    stepValues[index] = cutoff;
    DBG("Step " << index << " value " << cutoff);
}

void StepSequencer::setNumActiveSteps(int n) {
    if (currentStep > n - 1) {
        currentStep = 0;
    }
    numActiveSteps = n;

    //updateHalfCycle();

    DBG("Active steps: " << n);
}