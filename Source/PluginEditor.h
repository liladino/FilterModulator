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
    void setupSlider(juce::Slider& slider, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
        juce::AudioProcessorValueTreeState& vts, const juce::String& paramID);

    void setupKnob(juce::Slider& slider, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
        juce::AudioProcessorValueTreeState& vts, const juce::String& paramID);

    void setupButton(juce::Button& button, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& attachment,
        juce::AudioProcessorValueTreeState& vts, const juce::String& paramID);

    void addLabel(juce::Label& label, juce::AudioProcessorValueTreeState& vts, const juce::String& paramID, juce::Component& component);

    FilterModulatorAudioProcessor& audioProcessor;

    LabeledSlider cutoffFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        cutoffFrequencyAttachment;

    juce::Slider resonanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        resonanceAttachment;
    juce::Label resonanceLabel;

    juce::ToggleButton highpassButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
        highpassAttachment;
    juce::Label highpassButtonLabel;

    LabeledSlider modulatorSwitch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        modulatorSwitchAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterModulatorAudioProcessorEditor)
};
