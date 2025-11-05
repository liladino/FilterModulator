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

using Track = juce::Grid::TrackInfo;
using Fr = juce::Grid::Fr;

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

        slider.setBounds(bounds.withTop(20));
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

    lphp.setBounds(bounds.withTrimmedTop(25).withTrimmedBottom(25));
}


/***************
 * RateSetting *
 ***************/

RateSetting::RateSetting(juce::AudioProcessorValueTreeState& vts) : rate("Sec") {
    rateAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "rate", rate.knob);

    rate.knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    syncBPM.setButtonText("Sync BPM");

    syncBPMattachment =
        std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(vts, "syncBPM", syncBPM);

    label.setText("Rate", juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);

    bpmlabel.setText("BPM", juce::dontSendNotification);

    notelength.addItemList(juce::StringArray{ "1/3", "1/2","2/3","3/4","1","3/2","2","3","4" }, 1);

    notelength.setSelectedId(5);

    noteLengthAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(vts, "notelength", notelength);

    addAndMakeVisible(rate);
    addAndMakeVisible(syncBPM);
    addAndMakeVisible(bpmlabel);
    addAndMakeVisible(label);
    addAndMakeVisible(notelength);

    syncBPM.onClick = [this]()
        {
            bool bpmMode = syncBPM.getToggleState();
            rate.knob.setEnabled(!bpmMode);
        };
}

void RateSetting::resized() {
    juce::Grid grid;
    grid.templateColumns = { Track(Fr(1)), Track(Fr(1)) };
    grid.templateRows = { Track(Fr(1)), Track(Fr(3)), Track(Fr(4)), Track(Fr(3)) };
    grid.rowGap = juce::Grid::Px(10);
    grid.columnGap = juce::Grid::Px(10);

    grid.items.add(
        juce::GridItem(label).withArea(1, 1, juce::GridItem::Span(1), juce::GridItem::Span(2)),
        juce::GridItem(rate).withArea(2, 1, juce::GridItem::Span(3), juce::GridItem::Span(1)),
        juce::GridItem(syncBPM).withArea(2, 2),
        juce::GridItem(notelength).withArea(3, 2),
        juce::GridItem(bpmlabel).withArea(4, 2)
    );
    grid.performLayout(getLocalBounds());
}

void RateSetting::addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor) {
    vts.addParameterListener("rate", &audioProcessor);
    vts.addParameterListener("syncBPM", &audioProcessor);
    vts.addParameterListener("notelength", &audioProcessor);
}

void RateSetting::bpmChanged(float bpm) {
    DBG(bpm);
    if (bpm > 1) {
        bpmlabel.setText(std::to_string(static_cast<int>(bpm)) + std::string(" BPM"), juce::dontSendNotification);
    }
    else {
        bpmlabel.setText(" - BPM", juce::dontSendNotification);
    }
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

        label.setText("Sequencer", juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(label);
        addAndMakeVisible(slider);
    }
    ids.insert("numberOfSteps");
    numberAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "numberOfSteps", number.slider);

    addAndMakeVisible(number);
}

void Sequencer::resized() {
    juce::Grid grid;
    grid.templateColumns = { Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)) };
    grid.templateRows = { Track(Fr(1)), Track(Fr(4)), Track(Fr(4)), Track(Fr(4)), Track(Fr(4)) };
    grid.rowGap = juce::Grid::Px(10);
    grid.columnGap = juce::Grid::Px(10);

    grid.items.add(juce::GridItem(label).withArea(1, 1, juce::GridItem::Span(1), juce::GridItem::Span(4)));

    grid.items.add(
        juce::GridItem(number).withArea(1, 5, juce::GridItem::Span(5), juce::GridItem::Span(1))
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

        label.setText("Oscillator", juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(button);
    }
    waveFormButt[0].setToggleState(true, juce::dontSendNotification);
    waveFormButt[0].setButtonText("Sin");
    waveFormButt[1].setButtonText("Square");
    waveFormButt[2].setButtonText("Saw");
    waveFormButt[3].setButtonText("Saw * (-1)");

    widthAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "lfowidth", width.slider);
    addAndMakeVisible(width);
    addAndMakeVisible(label);
}

void LFOModulator::addListener(juce::AudioProcessorValueTreeState& vts, FilterModulatorAudioProcessor& audioProcessor) {
    vts.addParameterListener("lfowidth", &audioProcessor);
    for (int i = 0; i < 4; i++)
        vts.addParameterListener("wavebutton" + std::to_string(i), &audioProcessor);
}

void LFOModulator::resized() {
    juce::Grid grid;
    grid.templateColumns = { Track(Fr(1)), Track(Fr(1)) };
    grid.templateRows = { Track(Fr(1)), Track(Fr(7)), Track(Fr(7)), Track(Fr(7)) };
    grid.rowGap = juce::Grid::Px(10);
    grid.columnGap = juce::Grid::Px(10);

    grid.items.add(juce::GridItem(label).withArea(1, 1, juce::GridItem::Span(1), juce::GridItem::Span(2)));

    //grid.alignContent = juce::Grid::AlignContent::start;
    //grid.alignItems = juce::Grid::AlignItems::start;

    grid.items.add(
        juce::GridItem(width).withArea(4, 1, juce::GridItem::Span(1), juce::GridItem::Span(2))
    );

    for (auto& button : waveFormButt) {
        grid.items.add(juce::GridItem(button));
    }
    grid.performLayout(getLocalBounds());
}

