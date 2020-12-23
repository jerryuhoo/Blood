/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/Distortion.h"
#include "DSP/Delay.h"
#include "DSP/WidthProcessor.h"
#include "Preset.h"
#include "Multiband/FFTProcessor.h"
#include "GUI/InterfaceDefines.h"

//#define COLOUR1 Colour(244, 208, 63)
//#define COLOUR6 Colour(45, 40, 40)
//==============================================================================
/**
*/

// temporary use this method. This is not ideal. You should put your visualiser in Editor not in Processor
// probably use fifo
//class Visualiser : public juce::AudioVisualiserComponent
//{
//public:
//    Visualiser() : AudioVisualiserComponent(2)
//    {
//        setBufferSize(128);
//        setSamplesPerBlock(16);
//        setColours(COLOUR6, COLOUR1);
//        setBoundsInset(juce::BorderSize<int>(2, 2, 2, 2));
//    }
//
//    void paintChannel(juce::Graphics &g, juce::Rectangle<float> area,
//                      const juce::Range<float> *levels, int numLevels, int nextSample) override
//    {
//        juce::Path p;
//        getChannelAsPath(p, levels, numLevels, nextSample);
//        juce::ColourGradient grad(COLOUR1, area.getX() + area.getWidth() / 2, area.getY() + area.getHeight() / 2,
//                                  COLOUR6, area.getX(), area.getY() + area.getHeight() / 2, true);
//        g.setGradientFill(grad);
//        g.fillPath(p, juce::AffineTransform::fromTargetPoints(0.0f, -1.0f, area.getX(), area.getY(),
//                                                              0.0f, 1.0f, area.getX(), area.getBottom(),
//                                                              (float)numLevels, -1.0f, area.getRight(), area.getY()));
//    }
//};

class FireAudioProcessor : public juce::AudioProcessor
{

public:
    //==============================================================================
    FireAudioProcessor();
    ~FireAudioProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    // Rectification
    void updateRectification(juce::String recID, juce::SmoothedValue<float>& recSmoother, Distortion& distortionProcessor);

    // filter
    void updateFilter();

    bool isSlient(juce::AudioBuffer<float> buffer);

    // new drive after protection
    float getNewDrive();

    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    // ff meter
    // ff meter
    foleys::LevelMeterSource &getInputMeterSource()
    {
        return inputMeterSource;
    }
    foleys::LevelMeterSource &getOutputMeterSource()
    {
        return outputMeterSource;
    }

    // temporary use this method. This is not ideal. You should put your visualiser in Editor not in Processor
    // probably use fifo
    //Visualiser visualiser;

    juce::Array<float> getHistoryArrayL();
    juce::Array<float> getHistoryArrayR();
    
    // save presets
    state::StateAB stateAB;
    state::StatePresets statePresets;
    
    float * getFFTData();
    int getFFTSize();//TMP!!!!!!!!!!!
    bool isFFTBlockReady();
    void processFFT();
private:
    //==============================================================================
    
    // Oscilloscope
    juce::Array<float> historyArrayL;
    juce::Array<float> historyArrayR;
    int historyLength = 400;
    
    // Spectrum
    SpectrumProcessor spectrum_processor;
    
    // ff meter
    foleys::LevelMeterSource inputMeterSource;
    foleys::LevelMeterSource outputMeterSource;

    // dry audio buffer
    juce::AudioBuffer<float> dryBuffer;
    // dsp::AudioBlock<float> blockOutput;
    juce::dsp::ProcessSpec spec;

    // filter
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filterIIR;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filterColor;

    // fix the artifacts (also called zipper noise)
    //float previousGainInput;
    float previousOutput1;
    float previousOutput2;
    float previousOutput3;
    float previousOutput4;
    float previousDrive1;
    float previousDrive2;
    float previousDrive3;
    float previousDrive4;
    float previousMix1;
    float previousMix2;
    float previousMix3;
    float previousMix4;
    float previousMix;
    float previousColor;
    float previousCutoff;
    
    float newDrive1;
    float newDrive2;
    float newDrive3;
    float newDrive4;

    juce::SmoothedValue<float> driveSmoother1;
    juce::SmoothedValue<float> driveSmoother2;
    juce::SmoothedValue<float> driveSmoother3;
    juce::SmoothedValue<float> driveSmoother4;
    juce::SmoothedValue<float> outputSmoother1;
    juce::SmoothedValue<float> outputSmoother2;
    juce::SmoothedValue<float> outputSmoother3;
    juce::SmoothedValue<float> outputSmoother4;
    juce::SmoothedValue<float> recSmoother1;
    juce::SmoothedValue<float> recSmoother2;
    juce::SmoothedValue<float> recSmoother3;
    juce::SmoothedValue<float> recSmoother4;
    juce::SmoothedValue<float> biasSmoother1;
    juce::SmoothedValue<float> biasSmoother2;
    juce::SmoothedValue<float> biasSmoother3;
    juce::SmoothedValue<float> biasSmoother4;
    juce::SmoothedValue<float> mixSmoother1;
    juce::SmoothedValue<float> mixSmoother2;
    juce::SmoothedValue<float> mixSmoother3;
    juce::SmoothedValue<float> mixSmoother4;
    juce::SmoothedValue<float> mixSmootherGlobal;
    juce::SmoothedValue<float> colorSmoother;
    juce::SmoothedValue<float> cutoffSmoother;
    
    juce::SmoothedValue<float> centralSmoother;
    juce::SmoothedValue<float> normalSmoother;

    // DSP Processors
    Distortion distortionProcessor1;
    Distortion distortionProcessor2;
    Distortion distortionProcessor3;
    Distortion distortionProcessor4;
    WidthProcessor widthProcessor;
    
    // oversampling
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling;   // normal use 2x
    std::unique_ptr<juce::dsp::Oversampling<float>> oversamplingHQ; // HQ use 4x
    int oversampleFactor = 1;

    // oversampling delay, set to dry buffer
    Delay mDelay{0};

    // mode 8 diode================
    juce::Array<float> inputTemp;
    float VdiodeL;
    float VdiodeR;
    float RiL;
    float RiR;
    VoltageSource VinL;
    VoltageSource VinR;
    Resistor R1L;
    Resistor R1R;
    Capacitor C1L;
    Capacitor C1R;
    Serie RCL;
    Serie RCR;
    Serie rootL;
    Serie rootR;
    // mode 9 diode=================

    // dsp::AudioBlock<float> blockOutput;

    // multiband dsp
    juce::dsp::LinkwitzRileyFilter<float> lowpass1, highpass1,
                                          lowpass2, highpass2,
                                          lowpass3, highpass3;
    
    juce::AudioBuffer<float> mBuffer1, mBuffer2, mBuffer3, mBuffer4;
    bool multibandState1 = true;
    bool multibandState2 = true;
    bool multibandState3 = true;
    bool multibandState4 = true;
    
    bool multibandFocus1 = true;
    bool multibandFocus2 = false;
    bool multibandFocus3 = false;
    bool multibandFocus4 = false;
    
    
    void processDistortion(juce::String modeID, juce::String recID, juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::SmoothedValue<float>& driveSmoother, juce::SmoothedValue<float>& recSmoother, Distortion& distortionProcessor);
    void setParams(juce::String modeID, juce::String driveID, juce::String safeID, juce::String outputID, juce::String mixID, juce::String biasID, juce::AudioBuffer<float>& buffer, Distortion& distortionProcessor, juce::SmoothedValue<float>& driveSmoother, juce::SmoothedValue<float>& outputSmoother, juce::SmoothedValue<float>& mixSmoother, juce::SmoothedValue<float>& biasSmoother);
    void normalize(juce::String modeID, juce::AudioBuffer<float>& buffer, int totalNumInputChannels, juce::SmoothedValue<float>& recSmoother, juce::SmoothedValue<float>& outputSmoother);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FireAudioProcessor)
};
