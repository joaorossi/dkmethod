#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MainComponent   : public Component
{
public:
    MainComponent();
    ~MainComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    const double sampleRate = 48000.0;
    const double samplePeriod = 1.0 / sampleRate;
    const int blockSize = 48000 * 5;

    dkm::Model model;
    dsp::Gain<double> input, midle, output;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
