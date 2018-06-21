/**
    BEGIN_JUCE_MODULE_DECLARATION

    ID:             juce_dkmethod
    vendor:         Rossi, J., Macak, J.
    version:        0.1
    name:           JUCE DK-Method
    description:    A JUCE module for DK-Method analog emulation.
    license:        GPLv3

    dependencies:       juce_core, juce_audio_basics, juce_dsp
    OSXFrameworks:      Accelerate
    minimumCppStandard: 14
    searchpaths:        libs/armadillo-code/include
    OSXLibs:            armadillo

    END_JUCE_MODULE_DECLARATION
*/

#pragma once
#define JUCE_DKMETHOD_H_INCLUDED

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <armadillo>

using namespace juce;

#include "Components/Component.h"
#include "Components/ComponentFactory.h"
#include "Model/Model.h"
