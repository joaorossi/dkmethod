#include "PluginProcessor.h"
#include "PluginEditor.h"

DkmethodPluginTestAudioProcessor::DkmethodPluginTestAudioProcessor() :
    AudioProcessor (BusesProperties().withInput  ("Input",  AudioChannelSet::mono(), true)
                                     .withOutput ("Output", AudioChannelSet::mono(), true)),
    parameters (*this, nullptr)
{
    double R1, R2, R3, R4, R5;
    R1 = 68000.0;
    R2 = 1000000.0;
    R3 = 2700.0;
    R4 = 100000.0;
    R5 = 1000000.0;

    double C1, C2, C3, C4;
    C1 = 0.00000002;
    C2 = 0.00002;
    C3 = 0.00000002;
    C4 = 0.000000000002;

    model0.addComponent (dkm::ComponentFactory::makeResistor (R1, "R1", 2, 3));
    model0.addComponent (dkm::ComponentFactory::makeResistor (R2, "R2", 3, 0));
    model0.addComponent (dkm::ComponentFactory::makeResistor (R3, "R3", 4, 0));
    model0.addComponent (dkm::ComponentFactory::makeResistor (R4, "R4", 5, 6));
    model0.addComponent (dkm::ComponentFactory::makeResistor (R5, "R5", 7, 0));
    model0.addComponent (dkm::ComponentFactory::makeCapacitor (C1, "C1", 1, 2));
    model0.addComponent (dkm::ComponentFactory::makeCapacitor (C2, "C2", 4, 0));
    model0.addComponent (dkm::ComponentFactory::makeCapacitor (C3, "C3", 5, 7));
    model0.addComponent (dkm::ComponentFactory::makeCapacitor (C4, "C4", 3, 5));
    model0.addComponent (dkm::ComponentFactory::makeTriode ("T1", 3, 5, 4));
    model0.addComponent (dkm::ComponentFactory::makeInput ("In", 1, 0));
    model0.addComponent (dkm::ComponentFactory::makeInput ("Vps", 6, 0));
    model0.addComponent (dkm::ComponentFactory::makeOutput ("Out", 7, 0));

    model1.addComponent (dkm::ComponentFactory::makeResistor (R1, "R1", 2, 3));
    model1.addComponent (dkm::ComponentFactory::makeResistor (R2, "R2", 3, 0));
    model1.addComponent (dkm::ComponentFactory::makeResistor (R3, "R3", 4, 0));
    model1.addComponent (dkm::ComponentFactory::makeResistor (R4, "R4", 5, 6));
    model1.addComponent (dkm::ComponentFactory::makeResistor (R5, "R5", 7, 0));
    model1.addComponent (dkm::ComponentFactory::makeCapacitor (C1, "C1", 1, 2));
    model1.addComponent (dkm::ComponentFactory::makeCapacitor (C2, "C2", 4, 0));
    model1.addComponent (dkm::ComponentFactory::makeCapacitor (C3, "C3", 5, 7));
    model1.addComponent (dkm::ComponentFactory::makeCapacitor (C4, "C4", 3, 5));
    model1.addComponent (dkm::ComponentFactory::makeTriode ("T1", 3, 5, 4));
    model1.addComponent (dkm::ComponentFactory::makeInput ("In", 1, 0));
    model1.addComponent (dkm::ComponentFactory::makeInput ("Vps", 6, 0));
    model1.addComponent (dkm::ComponentFactory::makeOutput ("Out", 7, 0));

    driveGain.setGainLinear (1.0);
    driveGain.setRampDurationSeconds (0.1);
    interGain.setGainLinear (1.0 / 20.0);
    outputGain.setGainLinear (1.0 / 100.0);
    volumeGain.setGainLinear (1.0);
    volumeGain.setRampDurationSeconds (0.1);

    parameters.createAndAddParameter ("drive", "Drive", "Drive", { -24.f, 24.f, 0.1f }, 0.f, nullptr, nullptr);
    parameters.createAndAddParameter ("volume", "Volume", "Volume", { -24.f, 24.f, 0.1f }, 0.f, nullptr, nullptr);
    parameters.state = ValueTree (getName());
    parameters.addParameterListener ("drive", this);
    parameters.addParameterListener ("volume", this);
}

void DkmethodPluginTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec = { sampleRate, (uint32) samplesPerBlock, 1 };

    model0.prepare (spec);
    model1.prepare (spec);
    driveGain.prepare (spec);
    interGain.prepare (spec);
    outputGain.prepare (spec);
    volumeGain.prepare (spec);

    modelBuffer.setSize (2, samplesPerBlock);
}

void DkmethodPluginTestAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
    ScopedNoDenormals noDenormals;

    update();

    for (int n = 0; n < buffer.getNumSamples(); ++n)
    {
        modelBuffer.setSample (0, n, (double) buffer.getSample (0, n)); // Lousy convertion :/
        modelBuffer.setSample (1, n, 300.0); // Vps
    }

    dsp::AudioBlock<double> modelBlock (modelBuffer);
    dsp::ProcessContextReplacing<double> modelContext (modelBlock);

    dsp::AudioBlock<double> signalBlock = modelBlock.getSubsetChannelBlock (0, 1);
    dsp::ProcessContextReplacing<double> signalContext (signalBlock);

    driveGain.process (signalContext);
    model0.process (modelContext);
    interGain.process (signalContext);
    model1.process (modelContext);
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
