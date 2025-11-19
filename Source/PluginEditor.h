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

    juce::ComboBox filterMode;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        filterModeAttachment;

    LabeledSlider modulatorSwitch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        modulatorSwitchAttachment;

    Sequencer sequencerUI;
    RateSetting rate;
    LFOModulator oscillatorUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterModulatorAudioProcessorEditor)
};
