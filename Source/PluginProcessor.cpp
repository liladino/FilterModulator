/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CustomUI.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    /*params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HpLpMode", "Highpass",
        juce::NormalisableRange{ 0.f, 1.f, 1.f }, 0.f));*/

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "filterMode", "Note Length",
        juce::StringArray{ "BW Lowpass", "BW Highpass", "BW Notch", "Moog Lowpass", "Moog Highpass", "Moog Bandpass" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "resonance", "Resonance",
        juce::NormalisableRange{ 0.5f, 5.f, 0.0005f, 0.32f }, 1.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "cutoff", "Cutoff",
        juce::NormalisableRange{ 25.f, 20000.f, 0.05f, 0.25f }, 500.f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "modswitch", "Modulator switch",
        juce::NormalisableRange{ 0.f, 2.f, 1.f }, 0.f));

    //Sequencer
    for (int i = 0; i < 16; i++) {
        std::string id = Sequencer::knobNameBuilder("seqMod", i);
        std::string name = Sequencer::knobNameBuilder("Sequencer", i);
        
        DBG(id);
        //DBG(name);

        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            id, name, juce::NormalisableRange{ 25.f, 20000.f, 0.1f, 0.2f }, 500.f));
    }

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "numberOfSteps", "# active",
        juce::NormalisableRange{ 1.f, 16.f, 1.f }, 2.f));

    //Oscillator
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfowidth", "LFO width",
        juce::NormalisableRange{ 1.f, 24.f, 1.f }, 1.f));

    for (int i = 0; i < 4; i++) {
        std::string id = "wavebutton" + std::to_string(i);
        std::string name = "wavebutton" + std::to_string(i);

        DBG(id);

        params.push_back(std::make_unique<juce::AudioParameterBool>(
            id, name, i == 0));
    }

    //rate
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "rate", "Rate sec",
        juce::NormalisableRange{ 0.2f, 5.f, 0.1f }, 1.f));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "syncBPM", "Sync BPM", false));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "notelength", "Note Length",
        juce::StringArray{ "1/3", "1/2","2/3","3/4","1","3/2","2","3","4" }, 4 ));


    return { params.begin(), params.end() };
}

float bpmSet(juce::AudioPlayHead* playHead) {
    if (playHead == nullptr) {
        return 0;
    }
    DBG("playhead non zero");
    juce::AudioPlayHead::CurrentPositionInfo posInfo;

    if (!(playHead->getCurrentPosition(posInfo))) {
        return 0;
    }

    DBG("BPM " << posInfo.bpm);
    return posInfo.bpm;
}

void FilterModulatorAudioProcessor::parameterChanged(const juce::String& paramID, float newValue) {
    DBG("parameterChanged() called " << paramID);
    if (paramID == "filterMode") {
        const int val = static_cast<int>(newValue);
        DBG("filterMode: " << val);

        /* Modok : "BW Lowpass", "BW Highpass", "Moog Lowpass", "Moog Highpass"
         */

        switch (val) {
        default: engine.setFilterMode(FilterEngine::FilterMode::BWLowPass); break;
        case 1: engine.setFilterMode(FilterEngine::FilterMode::BWHighPass); break;
        case 2: engine.setFilterMode(FilterEngine::FilterMode::BWNotch); break;
        case 3: engine.setFilterMode(FilterEngine::FilterMode::MoogLowPass); break;
        case 4: engine.setFilterMode(FilterEngine::FilterMode::MoogHighPass); break;
        case 5: engine.setFilterMode(FilterEngine::FilterMode::MoogBandPass); break;
        }
    }
    else if (paramID == "cutoff") {
        engine.setCutoff(newValue);
        DBG("cutoff");
    }
    else if (paramID == "resonance") {
        engine.setResonance(newValue);
        DBG("resonance");
    }
    else if (paramID == "modswitch") {
        if (newValue < 0.5f) {
            engine.setModulator(FilterEngine::ModulatorMode::Off);
            DBG("modulator off");
        }
        else if (newValue < 1.5f) {
            engine.setModulator(FilterEngine::ModulatorMode::LFO);
            DBG("modulator LFO");
        }
        else {
            engine.setModulator(FilterEngine::ModulatorMode::Seq);
            DBG("modulator seq");
        }
    }
    else if (sequencerKnobIndexMap.count(paramID.toStdString()) > 0) {
        engine.setSequencerStep(sequencerKnobIndexMap[paramID.toStdString()], newValue);
    }
    else if (paramID == "numberOfSteps") {
        engine.setSequencerNum(newValue);
    }
    else if (paramID == "lfowidth") {
        engine.setLFOwidth(newValue);
    }
    else if (paramID.dropLastCharacters(1) == "wavebutton") {
        switch (paramID.getLastCharacter()) {
        case '1':
            engine.setModWaveType(WaveGenerator::WaveType::Square);
            break;
        case '2':
            engine.setModWaveType(WaveGenerator::WaveType::Saw);
            break;
        case '3':
            engine.setModWaveType(WaveGenerator::WaveType::MinusSaw);
            break;
        default:
            engine.setModWaveType(WaveGenerator::WaveType::Sin);
            break;
        }
    }
    else if (paramID == "rate") {
        engine.setRate(1.f / newValue);
    }
    else if (paramID == "syncBPM") {
        if (newValue > 0.5f) {
            auto* playHead = getPlayHead();
            float val = bpmSet(playHead);
            if (val > 1) {
                engine.syncToBPM(val);
                listener->bpmChanged(val);
            }
            else {
                engine.syncToBPM(60); //1 Hz dummy value
                listener->bpmChanged(60);
            }
        }
        else {
            listener->bpmChanged(0);
            engine.setRate();
        }
    }
    else if (paramID == "notelength") {
        const int val = static_cast<int>(newValue);
        DBG("Notelength: " << val);

        /* Note lengths : "1/3", "1/2", "2/3", "3/4", "1", "3/2", "2", "3", "4"
         * Reciprokokat veszek, hogy BPM erteknek feleljen meg a hanghossz
         */

        switch (val) {
            case 0:  engine.setBPMnoteLength(3); break;
            case 1:  engine.setBPMnoteLength(2); break;
            case 2:  engine.setBPMnoteLength(3.f / 2); break;
            case 3:  engine.setBPMnoteLength(4.f / 3); break;
            default: engine.setBPMnoteLength(1); break;
            case 5:  engine.setBPMnoteLength(2.f / 3); break;
            case 6:  engine.setBPMnoteLength(1.f / 2); break;
            case 7:  engine.setBPMnoteLength(1.f / 3); break;
            case 8:  engine.setBPMnoteLength(1.f / 4); break;
        }
    }
}


//==============================================================================
FilterModulatorAudioProcessor::FilterModulatorAudioProcessor()
      : AudioProcessor (BusesProperties()
                .withInput("Input",  juce::AudioChannelSet::stereo(), true)
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
        parameters(*this, nullptr, juce::Identifier("FilterModulatorPlugin"), createParameterLayout())
{
    engine.setFilterMode(FilterEngine::FilterMode::BWLowPass);
    engine.setResonance(1.f);
    engine.setCutoff(500.f);
    engine.setModulator(FilterEngine::ModulatorMode::Off);

    for (int i = 0; i < 16; i++) {
        std::string id = Sequencer::knobNameBuilder("seqMod", i);
        sequencerKnobIndexMap[id] = i;
    }
    //engine.bindCutoff(parameters.getRawParameterValue("cutoff"));
    //engine.bindRes(parameters.getRawParameterValue("resonance"));
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
