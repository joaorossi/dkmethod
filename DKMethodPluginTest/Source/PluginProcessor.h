#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DkmethodPluginTestAudioProcessor : public AudioProcessor,
                                         public AudioProcessorValueTreeState::Listener
{
public:
    DkmethodPluginTestAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void parameterChanged (const String&, float) override;

    AudioProcessorValueTreeState& getValueTreeState() { return parameters; }

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void releaseResources() override {}
    bool hasEditor() const override { return true; }
    const String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override { }
    const String getProgramName (int index) override { return {}; }
    void changeProgramName (int index, const String& newName) override { }
    //==============================================================================

private:
    AudioProcessorValueTreeState parameters;

    dkm::Model model;
    dsp::Gain<double> driveGain, outputGain, volumeGain;
    AudioBuffer<double> modelBuffer;

    void forceSteadyState (int blockSize);

    Atomic<int> shouldUpdate = true;
    void update();
    Atomic<double> newDrive = 0.f, newVolume = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DkmethodPluginTestAudioProcessor)
};
