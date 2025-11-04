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

struct LpHpSwitch : juce::Component {
    juce::Slider lphp;
    juce::Label above;
    juce::Label below;
    LpHpSwitch();
    void resized() override;
};

struct RateSetting : juce::Component, MyUIListener {
    LabeledKnob rate;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;

    juce::ToggleButton syncBPM;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncBPMattachment;

    juce::ComboBox notelength;
    juce::Label bpmlabel;
    RateSetting(juce::AudioProcessorValueTreeState& vts) : rate("Sec") {
        rateAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "rate", rate.knob);

        rate.knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

        syncBPM.setButtonText("Sync BPM");

        syncBPMattachment =
            std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(vts, "syncBPM", syncBPM);

        bpmlabel.setText("BPM", juce::dontSendNotification);

        notelength.addItem("1/3", 1);
        notelength.addItem("1/2", 2);
        notelength.addItem("2/3", 3);
        notelength.addItem("3/4", 4);
        notelength.addItem("1",   5);
        notelength.addItem("3/2", 6);
        notelength.addItem("2",   7);
        notelength.addItem("3",   8);
        notelength.addItem("4",   9);
        notelength.setSelectedId(5);

        addAndMakeVisible(rate);
        addAndMakeVisible(syncBPM);
        addAndMakeVisible(bpmlabel);
        addAndMakeVisible(notelength);

        syncBPM.onClick = [this]()
            {
                bool bpmMode = syncBPM.getToggleState();
                rate.knob.setEnabled(!bpmMode);
            };
    }

    void resized() override {
        juce::Grid grid;
        grid.templateColumns = { juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)) };
        grid.templateRows = { juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)) };
        grid.rowGap = juce::Grid::Px(10);
        grid.columnGap = juce::Grid::Px(10);

        grid.items.add(
            juce::GridItem(rate).withArea(1, 1, juce::GridItem::Span(3), juce::GridItem::Span(1)),
            juce::GridItem(syncBPM).withArea(1, 2),
            juce::GridItem(notelength).withArea(2, 2),
            juce::GridItem(bpmlabel).withArea(3, 2)
        );
        grid.performLayout(getLocalBounds());
    }

    void addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor) {
        vts.addParameterListener("rate", &audioProcessor);
        vts.addParameterListener("syncBPM", &audioProcessor);
    }

    virtual void bpmChanged(float bpm) override {
        DBG(bpm);
        if (bpm > 1) {
            bpmlabel.setText(std::to_string(bpm), juce::dontSendNotification);
        }
        else {
            bpmlabel.setText("BPM", juce::dontSendNotification);
        }
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
    std::array<juce::ToggleButton, 4> waveFormButt;
    std::array<std::unique_ptr < juce::AudioProcessorValueTreeState::ButtonAttachment>, 4>
        waveFormButtAttachment;

    LabeledSlider width;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment>
        widthAttachment;

    LFOModulator(juce::AudioProcessorValueTreeState& vts);

    void resized() override;
    void addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor);
};
