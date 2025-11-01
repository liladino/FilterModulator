/*
  ==============================================================================

    CustomUI.h
    Created: 29 Oct 2025 11:52:48pm
    Author:  BM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct LabeledSlider : juce::Component {
    juce::Slider slider;
    juce::Label label;
    juce::Label label2;
    juce::Label label3;
    bool textbox;
    LabeledSlider(const juce::String& name, bool textbox = true, bool vertical = false, const juce::String& name2 = "", const juce::String& name3 = "") : textbox(textbox) {
        label.setText(name, juce::dontSendNotification);
        label2.setText(name2, juce::dontSendNotification);
        label3.setText(name3, juce::dontSendNotification);
        addAndMakeVisible(label);
        addAndMakeVisible(label2);
        addAndMakeVisible(label3);
        addAndMakeVisible(slider);
        
        if (textbox) {
            slider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
        }
        else {
            slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
        }

        if (vertical) {
            slider.setSliderStyle(juce::Slider::LinearVertical);
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        }
        else {
            slider.setSliderStyle(juce::Slider::LinearHorizontal);
        }
    }

    void resized() override {
        auto bounds = getLocalBounds();

        if (textbox) {
            label.setJustificationType(juce::Justification::left);
            //label.setJustificationType(juce::Justification::bottomLeft);
            
            juce::Rectangle<int> labelArea = bounds.withHeight(20);//.withTrimmedTop(20);//
            //labelArea.translate(-80, 0);
            label.setBounds(labelArea);
            //label.setBounds(bounds);


            slider.setBounds(bounds);
        }
        else {
            label.setJustificationType(juce::Justification::centred);
            label2.setJustificationType(juce::Justification::left);
            label3.setJustificationType(juce::Justification::right);

            juce::Rectangle<int> labelArea = bounds.withTrimmedTop(20).withHeight(20);
            //labelArea.translate(-80, 0);
            label.setBounds(labelArea);
            label2.setBounds(labelArea);
            label3.setBounds(labelArea);

            slider.setBounds(bounds);//.withTrimmedTop(20));
        }
    }
};

struct LabeledKnob : juce::Component {
    juce::Slider knob;
    juce::Label label;
    LabeledKnob(const juce::String& name) {
        label.setText(name, juce::dontSendNotification);
        addAndMakeVisible(knob);
        addAndMakeVisible(label);

        knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
        knob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    }

    void resized() override {
        auto bounds = getLocalBounds();

        label.setJustificationType(juce::Justification::centred);

        juce::Rectangle<int> labelArea = bounds.withHeight(20);// .withTrimmedTop(20);
        //labelArea.translate(-80, 0);
        label.setBounds(labelArea);

        knob.setBounds(bounds.withTrimmedTop(20));
    }
};

struct LpHpSwitch : juce::Component {
    juce::Slider lphp;
    juce::Label above;
    juce::Label below;
    LpHpSwitch() {
        above.setText("Highpass", juce::dontSendNotification);
        below.setText("Lowpass", juce::dontSendNotification);
        addAndMakeVisible(above);
        addAndMakeVisible(below);
        addAndMakeVisible(lphp);
        lphp.setSliderStyle(juce::Slider::LinearVertical);
        lphp.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    }

    void resized() override {
        auto bounds = getLocalBounds();

        above.setJustificationType(juce::Justification::centred);
        below.setJustificationType(juce::Justification::centredBottom);

        juce::Rectangle<int> labelArea = bounds.withHeight(20);// .withTrimmedTop(20);
        above.setBounds(labelArea);

        //above.setBounds(bounds);
        below.setBounds(bounds);

        lphp.setBounds(bounds.withTrimmedTop(20).withTrimmedBottom(20));
    }
};

//TODO finish
struct RateSetting : juce::Component
{
    juce::Label rate;
    RateSetting()
    {
        rate.setText("Rate", juce::dontSendNotification);
        addAndMakeVisible(rate);
    }

    void resized() override
    {
        rate.setBounds(getLocalBounds());
    }
};


struct Sequencer : juce::Component
{
    std::array<juce::Slider, 16> freqKnobs;
    LabeledSlider number;
    Sequencer() : number("#", true, true)
    {
        for (auto& slider : freqKnobs)
        {
            slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
            addAndMakeVisible(slider);
        }
        addAndMakeVisible(number);
    }

    void resized() override
    {
        juce::Grid grid;
        grid.templateColumns = { juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)) };
        grid.templateRows = { juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)) };
        grid.rowGap = juce::Grid::Px(10);
        grid.columnGap = juce::Grid::Px(10);

        grid.items.add(
            juce::GridItem(number).withArea(1, 5, juce::GridItem::Span(4), juce::GridItem::Span(1))
        );

        for (auto& slider : freqKnobs)
            grid.items.add(juce::GridItem(slider));

        grid.performLayout(getLocalBounds());
    }
};

struct LFOModulator : juce::Component
{
    std::array<juce::ToggleButton, 4> waveForm;
    LabeledSlider width;
    LFOModulator() : width("LFO width", true)
    {
        for (auto& button : waveForm)
        {
            button.setRadioGroupId(1, juce::dontSendNotification);
            addAndMakeVisible(button);
        }
        addAndMakeVisible(width);
    }

    void resized() override
    {
        juce::Grid grid;
        grid.templateColumns = { juce::Grid::TrackInfo(juce::Grid::Fr(1))
            , juce::Grid::TrackInfo(juce::Grid::Fr(1))
        };
        grid.templateRows = { juce::Grid::TrackInfo(juce::Grid::Fr(1))
            , juce::Grid::TrackInfo(juce::Grid::Fr(1))
            , juce::Grid::TrackInfo(juce::Grid::Fr(1))
        };
        grid.rowGap = juce::Grid::Px(10);
        grid.columnGap = juce::Grid::Px(10);

        grid.items.add(
            juce::GridItem(width).withArea(3, 1, juce::GridItem::Span(1), juce::GridItem::Span(2))
        );

        for (auto& button : waveForm)
            grid.items.add(juce::GridItem(button));

        grid.performLayout(getLocalBounds());
    }
};
