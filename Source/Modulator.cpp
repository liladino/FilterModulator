/*
  ==============================================================================

    Modulator.cpp
    Created: 23 Oct 2025 2:17:00pm
    Author:  BM

  ==============================================================================
*/

#include "Modulator.h"

/*****************
 * StepSequencer *
 *****************/

StepSequencer::StepSequencer() {
    for (int i = 0; i < 16; i++) stepValues[i] = 500.f;

    samplesPerCycle = static_cast<int>(sampleRate / rate);
}

void StepSequencer::prepareToPlay(float sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    setRate(rate); //re-calculate samplesPerHalfCycle if sampleRate may have changed
}

float StepSequencer::getModulatedCutoff(float cutoff, int samplesPerBlock) {
    sampleCount += samplesPerBlock;

    if (sampleCount >= samplesPerCycle) {
        currentStep = (currentStep + 1) % numActiveSteps;
        sampleCount = 0;

        DBG("Step sequencer stepped " << currentStep << ' ' << samplesPerCycle);
    }

    return stepValues[currentStep];
}

void StepSequencer::setRate(float rateHz) {
    rate = rateHz;
    updateCycle();
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

/*****************
 * WaveGenerator *
 *****************/

void WaveGenerator::prepareToPlay(float sampleRate) {
    this->sampleRate = sampleRate;
    updateIncrement();
}

void WaveGenerator::setFrequency(float newFreq) {
    frequency = newFreq;
    updateIncrement();
}

float WaveGenerator::getNextValue(int samplesPerBlock) {
    float value = 0;
    switch (type) {
    case Sin:
        value = std::sin(2.0f * PI * phase);
        break;
    case Square:
        value = (phase < 0.5 ? 1 : -1);
        break;
    case Triangle:
        value = 4.0f * fabs(phase - 0.5f) - 1.0f;
        break;
    case Saw:
        value = -2.0 * phase + 1.0;
        break;
    }
    phase += samplesPerBlock * phaseIncrement;
    if (phase >= 1.0f) {
        phase = phase - floor(phase);
    }
    return value;
}

void WaveGenerator::setWaveType(WaveType wt) {
    type = wt;
}

void WaveGenerator::updateIncrement() {
    phaseIncrement = static_cast<double>(frequency) / static_cast<double>(sampleRate);
}

/**************
 * Oscillator *
 **************/
Oscillator::Oscillator() {
    depth = semitone;
    wgenerator.prepareToPlay(sampleRate);
}
void Oscillator::prepareToPlay(float sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    wgenerator.prepareToPlay(sampleRate);
    wgenerator.setFrequency(rate);
}

void Oscillator::setWaveType(WaveGenerator::WaveType type) {
    wgenerator.setWaveType(type);
}

void Oscillator::setDepth(float semitoneDepth) {
    //depth = (2^(1/12))^semitoneDepth
    depth = pow(semitone, semitoneDepth);
    DBG("depth = " << semitoneDepth << " multiplier: " << depth);
}

void Oscillator::setRate(float rateHz) {
    rate = rateHz;
    wgenerator.setFrequency(rate);
}

float Oscillator::getModulatedCutoff(float cutoff, int samplesPerBlock) {
    return std::clamp(cutoff * pow(depth, wgenerator.getNextValue(samplesPerBlock)), 21.f, 19990.f);
}
