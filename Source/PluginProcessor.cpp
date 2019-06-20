/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WholeBunchOfFiltersAudioProcessor::WholeBunchOfFiltersAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
        mStateTree(*this, nullptr, "ParameterState", {
            std::make_unique<AudioParameterFloat> ("cutoff", "Cutoff",
                NormalisableRange<float> (20.0f, 20000.0f, 0.001f), mCutoff),
            std::make_unique<AudioParameterInt> ("filterType", "FilterType", 1, 4, mFilterSelection),
        })
{
}

WholeBunchOfFiltersAudioProcessor::~WholeBunchOfFiltersAudioProcessor()
{
}

//==============================================================================
const String WholeBunchOfFiltersAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WholeBunchOfFiltersAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WholeBunchOfFiltersAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WholeBunchOfFiltersAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WholeBunchOfFiltersAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WholeBunchOfFiltersAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WholeBunchOfFiltersAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WholeBunchOfFiltersAudioProcessor::setCurrentProgram (int index)
{
}

const String WholeBunchOfFiltersAudioProcessor::getProgramName (int index)
{
    return {};
}

void WholeBunchOfFiltersAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WholeBunchOfFiltersAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;
    spec.sampleRate = mSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    mFirFilter.prepare (spec);
    mFirFilter.reset();
    
    setFilterType (*mStateTree.getRawParameterValue ("filterType"));
}

AudioProcessorValueTreeState& WholeBunchOfFiltersAudioProcessor::getValueTreeState()
{
    return mStateTree;
}

void WholeBunchOfFiltersAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WholeBunchOfFiltersAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void WholeBunchOfFiltersAudioProcessor::setFilterType (size_t filterSelection)
{
    float cutoff = *mStateTree.getRawParameterValue ("cutoff");
    size_t order = 21;
    
    switch (filterSelection)
    {
        // LPF Hamming Window
        case 1:
            *mFirFilter.state = *dsp::FilterDesign<float>::designFIRLowpassWindowMethod
                                (
                                    cutoff,
                                    mSampleRate,
                                    order,
                                    dsp::WindowingFunction<float>::hamming
                                );
            break;
        
        // LPF Blackman Harris Window
        case 2:
            *mFirFilter.state = *dsp::FilterDesign<float>::designFIRLowpassWindowMethod
                                (
                                    cutoff,
                                    mSampleRate,
                                    order,
                                    dsp::WindowingFunction<float>::blackmanHarris
                                );
            break;
        
        // LPF Kaiser Method
        case 3:
            *mFirFilter.state = *dsp::FilterDesign<float>::designFIRLowpassKaiserMethod
                                (
                                    cutoff,
                                    mSampleRate,
                                    0.25f,
                                    -100.0f
                                );
            break;
            
        // LPF Transition Method
        case 4:
            *mFirFilter.state = *dsp::FilterDesign<float>::designFIRLowpassTransitionMethod
                                (
                                    cutoff,
                                    mSampleRate,
                                    order,
                                    0.25f,
                                    1.0f
                                );
            break;
            
        default:
            *mFirFilter.state = *dsp::FilterDesign<float>::designFIRLowpassWindowMethod
                                (
                                    *mStateTree.getRawParameterValue("cutoff"),
                                    mSampleRate,
                                    order,
                                    dsp::WindowingFunction<float>::hamming
                                );
            break;
    }
}

void WholeBunchOfFiltersAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Process filter
    dsp::AudioBlock<float> block (buffer);
    
    int   filterSelection = *mStateTree.getRawParameterValue ("filterType");
    float cutoff          = *mStateTree.getRawParameterValue ("cutoff");
    
    setFilterType (filterSelection);
    mFirFilter.process(dsp::ProcessContextReplacing<float> (block));
}

//==============================================================================
bool WholeBunchOfFiltersAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WholeBunchOfFiltersAudioProcessor::createEditor()
{
    return new WholeBunchOfFiltersAudioProcessorEditor (*this);
}

//==============================================================================
void WholeBunchOfFiltersAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WholeBunchOfFiltersAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WholeBunchOfFiltersAudioProcessor();
}
