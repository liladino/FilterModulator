/*
  ==============================================================================

    CustomUI.h
    Created: 29 Oct 2025 11:52:48pm
    Author:  BM

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

struct LabeledSlider : juce::Component {
    juce::Slider slider;
    juce::Label label;
    juce::Label label2;
    juce::Label label3;
    bool textbox;
    LabeledSlider(const juce::String& name, bool textbox = true, bool vertical = false, const juce::String& name2 = "", const juce::String& name3 = "");
    void resized() override;
};

struct LabeledKnob : juce::Component {
    juce::Slider knob;
    juce::Label label;
    LabeledKnob(const juce::String& name);
    void resized() override;
};

struct LpHpSwitch : juce::Component {
    juce::Slider lphp;
    juce::Label above;
    juce::Label below;
    LpHpSwitch();
    void resized() override;
};

//TODO finish
struct RateSetting : juce::Component {
    juce::Label rate;
    RateSetting() {
        rate.setText("Rate", juce::dontSendNotification);
        addAndMakeVisible(rate);
    }

    void resized() override {
        rate.setBounds(getLocalBounds());
    }
};


struct Sequencer : juce::Component {
    static std::string knobNameBuilder(const std::string& name, int index) {
        std::stringstream idss;
        idss << name;
        idss << std::setw(2) << std::setfill('0') << index;
        std::string id; idss >> id;
        return id;
    }

    std::array<juce::Slider, 16> freqKnobs;
    LabeledSlider number;
    std::array<std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment>, 16>
        seqAttachmentArray;

    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment>
        numberAttachment;

    std::set<std::string> ids;

    Sequencer(juce::AudioProcessorValueTreeState& vts);

    void resized() override;

    void addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor);
};

struct LFOModulator : juce::Component {
    std::array<juce::ToggleButton, 4> waveForm;
    LabeledSlider width;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment>
        widthAttachment;

    LFOModulator(juce::AudioProcessorValueTreeState& vts);

    void resized() override;
    void addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor);
};
