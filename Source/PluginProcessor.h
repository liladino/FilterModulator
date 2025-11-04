/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterEngine.h"

//==============================================================================
/**
*/

class MyUIListener {
public:
    virtual void bpmChanged(float bpm) = 0;
};

class FilterModulatorAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    FilterModulatorAudioProcessor();
    ~FilterModulatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void parameterChanged(const juce::String& paramID, float newValue) override;

    void addMyUIListener(MyUIListener* listener){
        this->listener = listener;
    }
private:
    MyUIListener* listener = nullptr;
    juce::AudioProcessorValueTreeState parameters;
    /*std::atomic<float>* cutoffFrequencyParameter = nullptr;
    std::atomic<float>* highpassParameter = nullptr;

    std::unique_ptr<Filter> filter;
    std::unique_ptr<LowPassFilter> LP;
    std::unique_ptr<HighPassFilter> HP;*/

    FilterEngine engine;

    std::map<std::string, int> sequencerKnobIndexMap;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterModulatorAudioProcessor)
};
