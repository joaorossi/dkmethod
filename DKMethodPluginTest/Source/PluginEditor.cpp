#include "PluginProcessor.h"
#include "PluginEditor.h"

DkmethodPluginTestAudioProcessorEditor::DkmethodPluginTestAudioProcessorEditor (DkmethodPluginTestAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p),
    driveAtt (processor.getValueTreeState(), "drive", drive),
    volumeAtt (processor.getValueTreeState(), "volume", volume)
{
    drive.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    drive.setTextBoxStyle (Slider::TextBoxBelow, false, 40, 20);
    volume.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    volume.setTextBoxStyle (Slider::TextBoxBelow, false, 40, 20);

    addAndMakeVisible (drive);
    addAndMakeVisible (volume);

    setSize (400, 300);
}

DkmethodPluginTestAudioProcessorEditor::~DkmethodPluginTestAudioProcessorEditor()
{
}

void DkmethodPluginTestAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto w = bounds.getWidth();

    drive.setBounds (bounds.removeFromLeft (w / 2).withSizeKeepingCentre (175, 175));
    volume.setBounds (bounds.removeFromRight (w / 2).withSizeKeepingCentre (175, 175));
}
