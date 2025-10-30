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
    bool textbox;
    LabeledSlider(const juce::String& name, bool textbox = true, bool vertical = false) : textbox(textbox) {
        label.setText(name, juce::dontSendNotification);
        addAndMakeVisible(slider);
        addAndMakeVisible(label);
        
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
            
            slider.setBounds(bounds);
            juce::Rectangle<int> labelArea = bounds.withTrimmedTop(0).withHeight(20);
            //labelArea.translate(-80, 0);
            label.setBounds(labelArea);
            //label.setBounds(bounds);
        }
        else {
            label.setJustificationType(juce::Justification::centred);

            slider.setBounds(bounds);

            juce::Rectangle<int> labelArea = bounds.withTrimmedTop(0).withHeight(20);
            //labelArea.translate(-80, 0);
            label.setBounds(labelArea);
        }
    }
};

// TODO visual class for Sequencer
// TODO visual class for modulator
// TODO visual class for rate
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
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 10);
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
