#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class DkmethodPluginTestAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DkmethodPluginTestAudioProcessorEditor (DkmethodPluginTestAudioProcessor&);
    ~DkmethodPluginTestAudioProcessorEditor();

    void resized() override;

private:
    DkmethodPluginTestAudioProcessor& processor;

    Slider drive, volume;
    AudioProcessorValueTreeState::SliderAttachment driveAtt, volumeAtt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DkmethodPluginTestAudioProcessorEditor)
};
