/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================
FilterModulatorAudioProcessorEditor::FilterModulatorAudioProcessorEditor(FilterModulatorAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
        : AudioProcessorEditor(&p), audioProcessor(p), 
        cutoffFrequencySlider("Cutoff"), 
        modulatorSwitch("LFO", false, false, "Off", "Seq"),
        resonanceSlider("Resonance"),
        sequencerUI(vts),
        oscillatorUI(vts),
        rate(vts)
    {

    filterMode.addItemList(juce::StringArray{ "BW Lowpass", "BW Highpass", "BW Notch", "Moog Lowpass", "Moog Highpass", "Moog Bandpass" }, 1);
    filterModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(vts, "filterMode", filterMode);
    //filterMode.setSelectedId(1);
    addAndMakeVisible(filterMode);


    addAndMakeVisible(cutoffFrequencySlider);
    cutoffFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "cutoff", cutoffFrequencySlider.slider);
    
    addAndMakeVisible(modulatorSwitch);
    modulatorSwitchAttachment= std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "modswitch", modulatorSwitch.slider);

    addAndMakeVisible(resonanceSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "resonance", resonanceSlider.knob);

    //setupSlider(cutoffFrequencySlider, cutoffFrequencyLabel, cutoffFrequencyAttachment, vts, "cutoff");

    addAndMakeVisible(sequencerUI);
    addAndMakeVisible(oscillatorUI);
    addAndMakeVisible(rate);

    vts.addParameterListener("filterMode", &audioProcessor);
    vts.addParameterListener("cutoff", &audioProcessor);
    vts.addParameterListener("resonance", &audioProcessor);
    vts.addParameterListener("modswitch", &audioProcessor);
    
    sequencerUI.addListener(vts, audioProcessor);
    oscillatorUI.addListener(vts, audioProcessor);
    rate.addListener(vts, audioProcessor);
    
    audioProcessor.addMyUIListener(&rate);

    setSize(810, 630);
    setResizable(false, false);
    //setResizable(true, true);
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

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateColumns = { Track(Fr(1))
        , Track(Fr(1))
        , Track(Fr(4))
        , Track(Fr(2))
        //, juce::Grid::TrackInfo(juce::Grid::Fr(2)) 
    };
    grid.templateRows = { Track(Fr(4))
        , Track(Fr(3))
        , Track(Fr(11))
    };
    
    grid.items = {
        juce::GridItem(filterMode),
        juce::GridItem(resonanceSlider),
        juce::GridItem(cutoffFrequencySlider).withArea(1, 3, juce::GridItem::Span(1), juce::GridItem::Span(1)),
        juce::GridItem(rate),
        juce::GridItem(modulatorSwitch).withArea(2, 1, juce::GridItem::Span(1), juce::GridItem::Span(2)),
        juce::GridItem(oscillatorUI).withArea(3, 1, juce::GridItem::Span(1), juce::GridItem::Span(2)),
        juce::GridItem(sequencerUI).withArea(2, 3, juce::GridItem::Span(2), juce::GridItem::Span(2))
    };

    grid.rowGap = juce::Grid::Px(10);
    grid.columnGap = juce::Grid::Px(10);
    grid.justifyContent = juce::Grid::JustifyContent::center;
    grid.alignContent = juce::Grid::AlignContent::center;

    grid.performLayout(getLocalBounds().reduced(10));
    
}
