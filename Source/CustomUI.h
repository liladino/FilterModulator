/*
  ==============================================================================

    CustomUI.h
    Created: 29 Oct 2025 11:52:48pm
    Author:  BM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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

/*struct LpHpSwitch : juce::Component {
    juce::Slider lphp;
    juce::Label above;
    juce::Label below;
    LpHpSwitch();
    void resized() override;
};*/

struct RateSetting : juce::Component, MyUIListener {
    LabeledKnob rate;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;

    juce::ToggleButton syncBPM;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncBPMattachment;

    juce::ComboBox notelength;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> noteLengthAttachment;

    juce::Label label;
    juce::Label bpmlabel;

    RateSetting(juce::AudioProcessorValueTreeState& vts);
    void resized() override;
    void addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor);
    virtual void bpmChanged(float bpm) override;

    /*void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::darkorchid);
    }*/
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
    juce::Label label;

    std::array<std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment>, 16>
        seqAttachmentArray;

    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment>
        numberAttachment;

    std::set<std::string> ids;

    Sequencer(juce::AudioProcessorValueTreeState& vts);

    void resized() override;

    void addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor);

    /*void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::darkgoldenrod); 
        g.drawRect(getLocalBounds());
    }*/
};

struct LFOModulator : juce::Component {
    std::array<juce::ToggleButton, 4> waveFormButt;
    std::array<std::unique_ptr < juce::AudioProcessorValueTreeState::ButtonAttachment>, 4>
        waveFormButtAttachment;
    
    LabeledSlider width;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment>
        widthAttachment;
    
    juce::Label label;

    LFOModulator(juce::AudioProcessorValueTreeState& vts);

    void resized() override;
    void addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor);

    /*void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::darkslateblue);
    }*/
};


struct Separator : juce::Component
{
    enum Orientation { Vertical, Horizontal };
    Orientation orientation;

    Separator(Orientation o) : orientation(o) {}

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::white);

        if (orientation == Horizontal)
            g.fillRect(0, getHeight() / 2, getWidth(), 1);
        else
            g.fillRect(getWidth() / 2, 0, 1, getHeight());
    }
};