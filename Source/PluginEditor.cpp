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
        modulatorSwitch("LFO", false, false, "Off", "Seq"),
        resonanceSlider("Resonance") {

    addAndMakeVisible(cutoffFrequencySlider);
    cutoffFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "cutoff", cutoffFrequencySlider.slider);
    
    addAndMakeVisible(modulatorSwitch);
    modulatorSwitchAttachment= std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "modswitch", modulatorSwitch.slider);
    
    addAndMakeVisible(resonanceSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "resonance", resonanceSlider.knob);

    //setupSlider(cutoffFrequencySlider, cutoffFrequencyLabel, cutoffFrequencyAttachment, vts, "cutoff");

    addAndMakeVisible(sequencerUI);
    addAndMakeVisible(highpassSwitch);
    addAndMakeVisible(modulatorUI);
    addAndMakeVisible(rate);

    vts.addParameterListener("HpLpMode", &audioProcessor);
    highpassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "HpLpMode", highpassSwitch.lphp);

    setSize(800, 600);
    //setResizable(false, false);
    setResizable(true, true);
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
        , juce::Grid::TrackInfo(juce::Grid::Fr(4))
        , juce::Grid::TrackInfo(juce::Grid::Fr(2)) 
        //, juce::Grid::TrackInfo(juce::Grid::Fr(2)) 
    };
    grid.templateRows = { juce::Grid::TrackInfo(juce::Grid::Fr(1))
        , juce::Grid::TrackInfo(juce::Grid::Fr(1))
        , juce::Grid::TrackInfo(juce::Grid::Fr(3)) 
    };
    
    grid.items = {
        juce::GridItem(highpassSwitch),
        juce::GridItem(resonanceSlider),
        juce::GridItem(cutoffFrequencySlider).withArea(1, 3, juce::GridItem::Span(1), juce::GridItem::Span(1)),
        juce::GridItem(rate),
        juce::GridItem(modulatorSwitch).withArea(2, 1, juce::GridItem::Span(1), juce::GridItem::Span(2)),
        juce::GridItem(modulatorUI).withArea(3, 1, juce::GridItem::Span(1), juce::GridItem::Span(2)),
        juce::GridItem(sequencerUI).withArea(2, 3, juce::GridItem::Span(2), juce::GridItem::Span(2))
    };

    grid.rowGap = juce::Grid::Px(10);
    grid.columnGap = juce::Grid::Px(10);
    grid.justifyContent = juce::Grid::JustifyContent::center;
    grid.alignContent = juce::Grid::AlignContent::center;

    grid.performLayout(getLocalBounds().reduced(10));
    
}
