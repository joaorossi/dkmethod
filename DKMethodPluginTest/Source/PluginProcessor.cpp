#include "PluginProcessor.h"
#include "PluginEditor.h"

DkmethodPluginTestAudioProcessor::DkmethodPluginTestAudioProcessor() :
    AudioProcessor (BusesProperties().withInput  ("Input",  AudioChannelSet::mono(), true)
                                     .withOutput ("Output", AudioChannelSet::mono(), true)),
    parameters (*this, nullptr)
{
    // Resistors
    model.addComponent (dkm::ComponentFactory::makeResistor (68000.0,   "R1", 2, 3));
    model.addComponent (dkm::ComponentFactory::makeResistor (1000000.0, "R2", 3, 0));
    model.addComponent (dkm::ComponentFactory::makeResistor (2700.0,    "R3", 4, 0));
    model.addComponent (dkm::ComponentFactory::makeResistor (100000.0,  "R4", 5, 6));
    model.addComponent (dkm::ComponentFactory::makeResistor (1000000.0, "R5", 7, 0));

    // Capacitors
    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.00000002,     "C1", 1, 2));
    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.00002,        "C2", 4, 0));
    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.00000002,     "C3", 5, 7));
    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.000000000002, "C4", 3, 5));

    // Triode
    model.addComponent (dkm::ComponentFactory::makeTriode ("T1", 3, 5, 4));

    // Input and Output
    model.addComponent (dkm::ComponentFactory::makeInput ("In", 1, 0));
    model.addComponent (dkm::ComponentFactory::makeInput ("Vps", 6, 0));
    model.addComponent (dkm::ComponentFactory::makeOutput ("Out", 7, 0));

    driveGain.setGainLinear (1.0);
    driveGain.setRampDurationSeconds (0.1);
    outputGain.setGainLinear (1.0 / 125.0);
    volumeGain.setGainLinear (1.0);
    volumeGain.setRampDurationSeconds (0.1);

    parameters.createAndAddParameter ("drive", "Drive", "Drive", { -36.f, 36.f, 0.1f }, 0.f, nullptr, nullptr);
    parameters.createAndAddParameter ("volume", "Volume", "Volume", { -24.f, 24.f, 0.1f }, 0.f, nullptr, nullptr);
    parameters.state = ValueTree (getName());
    parameters.addParameterListener ("drive", this);
    parameters.addParameterListener ("volume", this);
}

void DkmethodPluginTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec = { sampleRate, (uint32) samplesPerBlock, 1 };

    model.prepare (spec);
    driveGain.prepare (spec);
    outputGain.prepare (spec);
    volumeGain.prepare (spec);

    modelBuffer.setSize (2, samplesPerBlock);

    forceSteadyState (samplesPerBlock);
}

void DkmethodPluginTestAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
    ScopedNoDenormals noDenormals;

    update();

    for (int n = 0; n < buffer.getNumSamples(); ++n)
        modelBuffer.setSample (0, n, (double) buffer.getSample (0, n)); // Lousy convertion :/

    // In and Vps
    dsp::AudioBlock<double> modelBlock (modelBuffer);
    dsp::ProcessContextReplacing<double> modelContext (modelBlock);

    // Only In
    dsp::AudioBlock<double> signalBlock = modelBlock.getSubsetChannelBlock (0, 1);
    dsp::ProcessContextReplacing<double> signalContext (signalBlock);

    driveGain.process (signalContext);
    model.process (modelContext);
    outputGain.process (signalContext);
    volumeGain.process (signalContext);

    for (int n = 0; n < buffer.getNumSamples(); ++n)
        buffer.setSample (0, n, (float) modelBuffer.getSample (0, n));
}

void DkmethodPluginTestAudioProcessor::update()
{
    if (shouldUpdate.compareAndSetBool (0, 1))
    {
        driveGain.setGainDecibels (newDrive.get());
        volumeGain.setGainDecibels (newVolume.get());
    }
}

void DkmethodPluginTestAudioProcessor::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == "drive")
        newDrive = (double) newValue;
    if (parameterID == "volume")
        newVolume = (double) newValue;

    shouldUpdate = 1;
}

void DkmethodPluginTestAudioProcessor::forceSteadyState (int blockSize)
{
    dsp::AudioBlock<double> block (modelBuffer);
    dsp::ProcessContextReplacing<double> context (block);

    auto vpsBlock = block.getSubsetChannelBlock (1, 1);
    vpsBlock.fill (300.0);

    for (int i = 0; i < 20; ++i)
    {
        modelBuffer.clear (0, 0, blockSize);
        model.process (context);
    }
}

void DkmethodPluginTestAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void DkmethodPluginTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
bool DkmethodPluginTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        || layouts.getMainInputChannelSet() != AudioChannelSet::mono())
        return false;
    return true;
}

AudioProcessorEditor* DkmethodPluginTestAudioProcessor::createEditor()
{
    return new DkmethodPluginTestAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DkmethodPluginTestAudioProcessor();
}
