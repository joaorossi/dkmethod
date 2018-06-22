#include "MainComponent.h"

MainComponent::MainComponent() :
    model (sampleRate)
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

    // 5 secs of sine wave and Vps at 300V
    AudioBuffer<double> buffer (2, blockSize);
    buffer.clear();
    for (int i = 0; i < blockSize; ++i)
    {
        buffer.setSample (0, i, 5.0 * std::cos (M_2_PI * 1000.0 * ((double) i) / 44100.0));
        buffer.setSample (1, i, 300.0);
    }

    dsp::AudioBlock<double> block (buffer);
    dsp::ProcessContextReplacing<double> context (block);
    dsp::ProcessSpec spec = { sampleRate, (uint32) blockSize, 1 };

    // Process audio
    model.prepare (spec);
    model.process (context);

    /** Prints the last 2000 samples as CSV on a file called result.txt, the data
        can be visualized with the ipython notebook inside the project directory.
    */
    {
        File f (File::getCurrentWorkingDirectory().getChildFile ("results.txt"));
        ScopedPointer<OutputStream> fos = f.createOutputStream();
        for (int i = blockSize - 2000; i < blockSize; ++i)
        {
            if (i == (blockSize - 1))
                fos->writeText (String (buffer.getSample (0, i)), false, false, nullptr);
            else
                fos->writeText (String (buffer.getSample (0, i)) + String (","), false, false, nullptr);
        }
    }
    setSize (300, 200);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::black);
    g.setFont (Font (22.f));
    g.drawText ("DK-Method for JUCE", getLocalBounds(), Justification::centred);
}

void MainComponent::resized()
{
}
