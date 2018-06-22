#include "MainComponent.h"

MainComponent::MainComponent() :
    model0 (sampleRate), model1 (sampleRate)
{
//    model.addComponent (dkm::ComponentFactory::makeResistor (1000000.0, "R1", 1, 0));
//    model.addComponent (dkm::ComponentFactory::makeResistor (68000.0, "R2", 1, 2));
//    model.addComponent (dkm::ComponentFactory::makeResistor (2700.0, "R3", 3, 0));
//    model.addComponent (dkm::ComponentFactory::makeResistor (100000.0, "R4", 5, 4));
//    model.addComponent (dkm::ComponentFactory::makeResistor (470000.0, "R5", 6, 7));
//    model.addComponent (dkm::ComponentFactory::makeResistor (500000.0, "R6a", 7, 8));
//    model.addComponent (dkm::ComponentFactory::makeResistor (500000.0, "R6b", 8, 0));
//    model.addComponent (dkm::ComponentFactory::makeResistor (10000.0, "R7", 11, 0));
//    model.addComponent (dkm::ComponentFactory::makeResistor (100000.0, "R8", 10, 9));
//    model.addComponent (dkm::ComponentFactory::makeResistor (470000.0, "R9", 12, 13));
//    model.addComponent (dkm::ComponentFactory::makeResistor (470000.0, "R10", 13, 0));
//    model.addComponent (dkm::ComponentFactory::makeResistor (820.0, "R11", 15, 0));
//    model.addComponent (dkm::ComponentFactory::makeResistor (100000.0, "R12", 16, 14));
//    model.addComponent (dkm::ComponentFactory::makeResistor (100000.0, "RL", 14, 10));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.00000068, "C1", 3, 0));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.0000000001, "C2", 4, 3));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.000000022, "C3", 4, 6));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.00000000047, "C4", 6, 7));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.000000001, "C5", 7, 8));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.000000022, "C6", 9, 12));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.00000000047, "C7", 12, 13));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.000000000002, "Cm1", 2, 4));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.000000000002, "Cm2", 8, 9));
//    model.addComponent (dkm::ComponentFactory::makeCapacitor (0.000000000002, "Cm3", 13, 14));
//    model.addComponent (dkm::ComponentFactory::makeTriode ("T1", 2, 4, 3));
//    model.addComponent (dkm::ComponentFactory::makeTriode ("T2", 8, 9, 11));
//    model.addComponent (dkm::ComponentFactory::makeTriode ("T3", 13, 14, 15));
//    model.addComponent (dkm::ComponentFactory::makeInput ("In", 1, 0));
//    model.addComponent (dkm::ComponentFactory::makeInput ("Vps1", 5, 0));
//    model.addComponent (dkm::ComponentFactory::makeInput ("Vps2", 10, 0));
//    model.addComponent (dkm::ComponentFactory::makeInput ("Vps3", 16, 0));
//    model.addComponent (dkm::ComponentFactory::makeOutput ("Out", 14, 0));

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

    AudioBuffer<double> buffer (2, blockSize);
    buffer.clear();

    for (int i = 0; i < blockSize; ++i)
    {
        buffer.setSample (0, i, std::cos (M_2_PI * 1000.0 * ((double) i) / 44100.0));
        buffer.setSample (1, i, 300.0);
    }

    input.setGainLinear (4.0);
    midle.setGainLinear (1.0 / 20.0);
    output.setGainLinear (1.0 / 100.0);

    dsp::AudioBlock<double> block (buffer);
    dsp::ProcessContextReplacing<double> context (block);

    auto audioBlock = block.getSubsetChannelBlock (0, 1);
    dsp::ProcessContextReplacing<double> audioContext (audioBlock);

    dsp::ProcessSpec spec = { sampleRate, (uint32) blockSize, 1 };
    model0.prepare (spec);
    model1.prepare (spec);
    input.prepare (spec);
    midle.prepare (spec);
    output.prepare (spec);

    input.process (audioContext);
    model0.process (context);
    midle.process (audioContext);
    model1.process (context);
    output.process (audioContext);

    {
        File f (File::getSpecialLocation (File::userDesktopDirectory).getChildFile ("dataset.txt"));
        ScopedPointer<OutputStream> fos = f.createOutputStream();
        for (int i = blockSize - 2000; i < blockSize; ++i)
        {
            if (i == (blockSize - 1))
                fos->writeText (String (buffer.getSample (0, i)), false, false, nullptr);
            else
                fos->writeText (String (buffer.getSample (0, i)) + String (","), false, false, nullptr);
        }
    }
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
}
