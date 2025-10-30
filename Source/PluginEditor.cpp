/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterModulatorAudioProcessorEditor::FilterModulatorAudioProcessorEditor(FilterModulatorAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
        : AudioProcessorEditor(&p), audioProcessor(p), 
        cutoffFrequencySlider("Cutoff"), 
        modulatorSwitch("Off    LFO    Seq", false), 
        resonanceSlider("Resonance", false) {

    addAndMakeVisible(cutoffFrequencySlider);
    cutoffFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "cutoff", cutoffFrequencySlider.slider);
    
    addAndMakeVisible(modulatorSwitch);
    modulatorSwitchAttachment= std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "modswitch", modulatorSwitch.slider);
    
    addAndMakeVisible(resonanceSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "resonance", resonanceSlider.slider);

    resonanceSlider.slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    resonanceSlider.slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    //setupSlider(cutoffFrequencySlider, cutoffFrequencyLabel, cutoffFrequencyAttachment, vts, "cutoff");

    addAndMakeVisible(sequencerKnobs);
    addAndMakeVisible(rate);

    setupButton(highpassButton, highpassButtonLabel, highpassAttachment, vts, "HpLpMode");
    vts.addParameterListener("HpLpMode", &audioProcessor);

    setSize(600, 600);
    //setResizable(false, false);
    setResizable(true, true);
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

    label.attachToComponent(&component, false);
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

    grid.templateColumns = { juce::Grid::TrackInfo(juce::Grid::Fr(1))
        , juce::Grid::TrackInfo(juce::Grid::Fr(1))
        , juce::Grid::TrackInfo(juce::Grid::Fr(1))
        , juce::Grid::TrackInfo(juce::Grid::Fr(1)) 
        , juce::Grid::TrackInfo(juce::Grid::Fr(1)) 
    };
    grid.templateRows = { juce::Grid::TrackInfo(juce::Grid::Fr(1))
        , juce::Grid::TrackInfo(juce::Grid::Fr(1))
        , juce::Grid::TrackInfo(juce::Grid::Fr(1)) 
    };
    
    grid.items = {
        juce::GridItem(highpassButton),
        juce::GridItem(resonanceSlider),
        juce::GridItem(cutoffFrequencySlider).withArea(1, 3, juce::GridItem::Span(1), juce::GridItem::Span(2)),
        juce::GridItem(rate),
        juce::GridItem(modulatorSwitch).withArea(2, 1, juce::GridItem::Span(1), juce::GridItem::Span(3)),
        juce::GridItem(sequencerKnobs).withArea(2, 4, juce::GridItem::Span(2), juce::GridItem::Span(2))
    };

    grid.rowGap = juce::Grid::Px(10);
    grid.columnGap = juce::Grid::Px(10);
    grid.justifyContent = juce::Grid::JustifyContent::center;
    grid.alignContent = juce::Grid::AlignContent::center;

    grid.performLayout(getLocalBounds());
    
}
