/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "cutoff", "Cutoff",
        juce::NormalisableRange{ 20.f, 20000.f, 0.1f, 0.2f }, 500.f));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "HpLpMode", "Highpass", false));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "resonance", "Resonance",
        juce::NormalisableRange{ 0.5f, 4.f, 0.0005f }, 1.f));

    /*
    Rezonancia:
        egy skalar szorzo, ami beallitja, hogy a ket biquad szuro 
        Q1 es Q2 ertekeit milyen s-sel szorozzuk (ref. Matlab).

        Q1 = 0.5411961
        Q2 = 1.306563

        (4edfoku kaszkados Butterwoth szuro ertekei)
        https://www.earlevel.com/main/2016/09/29/cascading-filters/
    */

    return { params.begin(), params.end() };
}

void FilterModulatorAudioProcessor::parameterChanged(const juce::String& paramID, float newValue) {
    if (paramID == "HpLpMode")
    {
        if (newValue < 0.5f) {
            engine.setFilterMode(FilterEngine::FilterMode::LowPass);
        }
        else {
            engine.setFilterMode(FilterEngine::FilterMode::HighPass);
        }
        DBG("parameterChanged() called Highpass/lowpass");
    }
}

//==============================================================================
FilterModulatorAudioProcessor::FilterModulatorAudioProcessor()
      : AudioProcessor (BusesProperties()
                .withInput("Input",  juce::AudioChannelSet::stereo(), true)
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
        parameters(*this, nullptr, juce::Identifier("FilterModulatorPlugin"), createParameterLayout())
{
    engine.bindCutoff(parameters.getRawParameterValue("cutoff"));
    engine.bindRes(parameters.getRawParameterValue("resonance"));
    engine.bindHighpass(parameters.getRawParameterValue("resonance"));
}

FilterModulatorAudioProcessor::~FilterModulatorAudioProcessor()
{
}

//==============================================================================
const juce::String FilterModulatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterModulatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterModulatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterModulatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterModulatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterModulatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterModulatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterModulatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FilterModulatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterModulatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FilterModulatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    engine.prepareToPlay(sampleRate, samplesPerBlock);
}

void FilterModulatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterModulatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FilterModulatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    engine.processBlock(buffer);
    
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

    }*/
}

//==============================================================================
bool FilterModulatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FilterModulatorAudioProcessor::createEditor()
{
    return new FilterModulatorAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void FilterModulatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FilterModulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterModulatorAudioProcessor();
}
