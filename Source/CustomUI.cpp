/*
  ==============================================================================

    CustomUI.cpp
    Created: 3 Nov 2025 6:42:26pm
    Author:  BM

  ==============================================================================
*/

#include "CustomUI.h"

/*****************
 * LabeledSlider *
 *****************/

LabeledSlider::LabeledSlider(const juce::String& name, bool textbox, bool vertical, const juce::String& name2, const juce::String& name3) : textbox(textbox) {
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

void LabeledSlider::resized() {
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

/***************
 * LabeledKnob *
 ***************/

LabeledKnob::LabeledKnob(const juce::String& name) {
    label.setText(name, juce::dontSendNotification);
    addAndMakeVisible(knob);
    addAndMakeVisible(label);

    knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    knob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
}

void LabeledKnob::resized() {
    auto bounds = getLocalBounds();

    label.setJustificationType(juce::Justification::centred);

    juce::Rectangle<int> labelArea = bounds.withHeight(20);// .withTrimmedTop(20);
    //labelArea.translate(-80, 0);
    label.setBounds(labelArea);

    knob.setBounds(bounds.withTrimmedTop(20));
}

/**************
 * LpHpSwitch *
 **************/

LpHpSwitch::LpHpSwitch() {
    above.setText("Highpass", juce::dontSendNotification);
    below.setText("Lowpass", juce::dontSendNotification);
    addAndMakeVisible(above);
    addAndMakeVisible(below);
    addAndMakeVisible(lphp);
    lphp.setSliderStyle(juce::Slider::LinearVertical);
    lphp.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
}

void LpHpSwitch::resized() {
    auto bounds = getLocalBounds();

    above.setJustificationType(juce::Justification::centred);
    below.setJustificationType(juce::Justification::centredBottom);

    juce::Rectangle<int> labelArea = bounds.withHeight(20);// .withTrimmedTop(20);
    above.setBounds(labelArea);

    //above.setBounds(bounds);
    below.setBounds(bounds);

    lphp.setBounds(bounds.withTrimmedTop(20).withTrimmedBottom(20));
}


/*************
 * Sequencer *
 *************/
Sequencer::Sequencer(juce::AudioProcessorValueTreeState& vts) : number("Active", true, true) {
    for (int i = 0; i < 16; i++) {
        std::string id = knobNameBuilder("seqMod", i);
        ids.insert(id);

        seqAttachmentArray[i] =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, id, freqKnobs[i]);

        auto& slider = freqKnobs[i];
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        addAndMakeVisible(slider);
    }
    ids.insert("numberOfSteps");
    numberAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "numberOfSteps", number.slider);

    addAndMakeVisible(number);
}

void Sequencer::resized() {
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

void Sequencer::addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor) {
    for (auto& x : ids) {
        vts.addParameterListener(x, &audioProcessor);
    }
}

/****************
 * LFOModulator *
 ****************/

LFOModulator::LFOModulator(juce::AudioProcessorValueTreeState& vts) : width("LFO width", true) {
    for (int i = 0; i < 4; i++) {
        auto& button = waveFormButt[i];
        button.setRadioGroupId(1, juce::dontSendNotification);

        waveFormButtAttachment[i] =
            std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(vts, "wavebutton" + std::to_string(i), button);

        addAndMakeVisible(button);
    }
    waveFormButt[0].setToggleState(true, juce::dontSendNotification);
    widthAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "lfowidth", width.slider);
    addAndMakeVisible(width);
}

void LFOModulator::addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor) {
    vts.addParameterListener("lfowidth", &audioProcessor);
    for (int i = 0; i < 4; i++)
        vts.addParameterListener("wavebutton" + std::to_string(i), &audioProcessor);
}

void LFOModulator::resized() {
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

    for (auto& button : waveFormButt) {
        grid.items.add(juce::GridItem(button));
    }
    grid.performLayout(getLocalBounds());
}

