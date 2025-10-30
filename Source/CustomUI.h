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
    LabeledSlider(const juce::String& name, bool textbox = true) : textbox(textbox) {
        label.setText(name, juce::dontSendNotification);
        addAndMakeVisible(slider);
        addAndMakeVisible(label);
        
        if (textbox) {
            slider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
        }
        else {
            slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
        }
        slider.setSliderStyle(juce::Slider::LinearHorizontal);
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