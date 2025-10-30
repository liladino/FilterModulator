/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterModulatorAudioProcessorEditor::FilterModulatorAudioProcessorEditor(FilterModulatorAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), audioProcessor(p), cutoffFrequencySlider("Cutoff"), modulatorSwitch("Off    LFO    Seq", false) {

    addAndMakeVisible(cutoffFrequencySlider);
    cutoffFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "cutoff", cutoffFrequencySlider.slider);
    
    addAndMakeVisible(modulatorSwitch);
    modulatorSwitchAttachment= std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "modswitch", modulatorSwitch.slider);

    //setupSlider(cutoffFrequencySlider, cutoffFrequencyLabel, cutoffFrequencyAttachment, vts, "cutoff");

    setupKnob(resonanceSlider, resonanceLabel, resonanceAttachment, vts, "resonance");

    setupButton(highpassButton, highpassButtonLabel, highpassAttachment, vts, "HpLpMode");
    vts.addParameterListener("HpLpMode", &audioProcessor);

    setSize(600, 600);
    //setResizable(false, false);
    setResizable(true, true);
}

void FilterModulatorAudioProcessorEditor::setupSlider(juce::Slider& slider, juce::Label& label, 
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
        juce::AudioProcessorValueTreeState& vts, const juce::String& paramID) {

    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, paramID, slider);

    addLabel(label, vts, paramID, slider);

}

void FilterModulatorAudioProcessorEditor::setupKnob(juce::Slider& slider, juce::Label& label, 
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
        juce::AudioProcessorValueTreeState& vts, const juce::String& paramID) {

    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, paramID, slider);

    addLabel(label, vts, paramID, slider);
}

void FilterModulatorAudioProcessorEditor::setupButton(juce::Button& button, juce::Label& label, 
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& attachment,
        juce::AudioProcessorValueTreeState& vts, const juce::String& paramID) {

    addAndMakeVisible(button);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(vts, paramID, button);

    addLabel(label, vts, paramID, button);
}

void FilterModulatorAudioProcessorEditor::addLabel(juce::Label& label, juce::AudioProcessorValueTreeState& vts, 
    const juce::String& paramID, juce::Component& component) {
    
    addAndMakeVisible(label);

    if (auto* param = vts.getParameter(paramID)) {
        label.setText(param->getName(64), juce::dontSendNotification);
    }
    else {
        label.setText(paramID, juce::dontSendNotification);
    }

    label.attachToComponent(&component, true);
}


FilterModulatorAudioProcessorEditor::~FilterModulatorAudioProcessorEditor()
{
}

//==============================================================================
void FilterModulatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void FilterModulatorAudioProcessorEditor::resized()
{
    juce::Grid grid;

    grid.templateColumns = { juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)) };
    grid.templateRows = { juce::Grid::TrackInfo(juce::Grid::Fr(1)), juce::Grid::TrackInfo(juce::Grid::Fr(1)) };
    
    grid.items = {
        juce::GridItem(highpassButton),
        juce::GridItem(resonanceSlider),
        juce::GridItem(cutoffFrequencySlider),
        juce::GridItem(modulatorSwitch)
    };

    grid.rowGap = juce::Grid::Px(10);
    grid.columnGap = juce::Grid::Px(10);
    grid.justifyContent = juce::Grid::JustifyContent::center;
    grid.alignContent = juce::Grid::AlignContent::center;

    grid.performLayout(getLocalBounds());
    
}
