/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BloodAudioProcessor::BloodAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )

,treeState(*this,nullptr,"PARAMETERS",
{
    std::make_unique<AudioParameterFloat>("drive","Drive",NormalisableRange<float>(0.0f,100.0f,1.0f), 100.0f),
    std::make_unique<AudioParameterFloat>("range","Range",NormalisableRange<float>(0.0f,100.0f,1.0f), 100.0f),
    std::make_unique<AudioParameterFloat>("mix","Mix",NormalisableRange<float>(0.0f,1.0f,0.01f), 1.0f),
    std::make_unique<AudioParameterFloat>("gain","Gain",NormalisableRange<float>(-48.0f,6.0f,0.1f), 0.0f),
})
#endif
{
    
}


BloodAudioProcessor::~BloodAudioProcessor()
{
}

//==============================================================================
const String BloodAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BloodAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BloodAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BloodAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BloodAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BloodAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BloodAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BloodAudioProcessor::setCurrentProgram (int index)
{
}

const String BloodAudioProcessor::getProgramName (int index)
{
    return {};
}

void BloodAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BloodAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    auto sliderGainValue = treeState.getRawParameterValue("gain");
    previousGain = (Decibels::decibelsToGain(*sliderGainValue));
}

void BloodAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BloodAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BloodAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    auto driveValue = treeState.getRawParameterValue("drive");
    float drive = *driveValue;
//    auto rangeValue = treeState.getRawParameterValue("range");
//    float range = *rangeValue;
    auto mixValue = treeState.getRawParameterValue("mix");
    float mix = *mixValue;
    auto gainValue = treeState.getRawParameterValue("gain");
    float currentGain = Decibels::decibelsToGain(*gainValue);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        int menuChoice = 1;
        float thresh = 1.f;
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            
            auto input = channelData[sample];
            auto cleanOut = channelData[sample];
            input *= (1+drive/20);
            if (menuChoice == 1)
                //Hard Clipping
            {
                if (input > thresh)
                {
                    input = thresh;
                }
                else if (input < -thresh)
                {
                    input = -thresh;
                }
                else
                {
                    input = input;
                }
            }
            if (menuChoice == 2)
                //Soft Clipping Exp
            {
                if (input > thresh)
                {
                    input = 1.0f - expf(-input);
                }
                else
                {
                    input = -1.0f + expf(input);
                }
            }
            if (menuChoice == 3)
                //Half-Wave Rectifier
            {
                if (input > thresh)
                {
                    input = input;
                }
                else
                {
                    input = 0;
                }
            }
            channelData[sample] = ((1 - mix) * cleanOut) + (mix * input);
        
    
//            auto originalSig = *channelData;
//            *channelData *= (1+drive/100.f);
//            if (*channelData > 0) {
//                y = 1 - exp(-x);
//            }
//            else {
//                y = -1 + exp(x);
//            }
//            *channelData = (2.f/float_Pi)*atan(*channelData)*mix + originalSig*(1.f-mix);
//            channelData++;
        }
        // ..do something to the data...
    }

    
    if (currentGain == previousGain) {
        buffer.applyGain(currentGain);
    } else {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }
    
}



//==============================================================================
bool BloodAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BloodAudioProcessor::createEditor()
{
    return new BloodAudioProcessorEditor (*this);
}

//==============================================================================
void BloodAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    //MemoryOutputStream (destData, true).writeFloat (*gain);
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void BloodAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    //*gain = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (treeState.state.getType()))
            treeState.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BloodAudioProcessor();
}