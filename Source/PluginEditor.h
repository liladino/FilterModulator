/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomUI.h"

//==============================================================================
/**
*/
class FilterModulatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FilterModulatorAudioProcessorEditor(FilterModulatorAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~FilterModulatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    FilterModulatorAudioProcessor& audioProcessor;

    LabeledSlider cutoffFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        cutoffFrequencyAttachment;

    LabeledKnob resonanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        resonanceAttachment;

    LpHpSwitch highpassSwitch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        highpassAttachment;

    LabeledSlider modulatorSwitch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        modulatorSwitchAttachment;

    Sequencer sequencerUI;
    RateSetting rate;
    LFOModulator modulatorUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterModulatorAudioProcessorEditor)
};
