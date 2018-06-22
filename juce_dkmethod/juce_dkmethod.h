/**
    BEGIN_JUCE_MODULE_DECLARATION

    ID:             juce_dkmethod
    vendor:         Rossi, J., Macak, J.
    version:        0.1
    name:           JUCE DK-Method
    description:    A JUCE module for analog emulation using the DK-Method.
    license:        GPLv3

    dependencies:       juce_core, juce_audio_basics, juce_dsp
    OSXFrameworks:      Accelerate
    minimumCppStandard: 14

    END_JUCE_MODULE_DECLARATION
*/

#pragma once
#define JUCE_DKMETHOD_H_INCLUDED

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

/** For Mac OSX we can use Armadillo as header-only. */
#if JUCE_MAC
  #define ARMA_DONT_USE_WRAPPER
#endif

/** TODO - Windose (Unsupported for now :/)

    After some reading, looks like using OpenBLAS makes it a bit easier,
    since they offer pre-built binaries.
    https://www.openblas.net/

    But for LAPACK there's no way around.
    It has to be built with MinGW or Intel's Fortran Compiler,
    and I really don't know if linking will be possible.
    http://icl.cs.utk.edu/lapack-for-windows/lapack/

    There's also the option of using Intel MKL, but not sure about that as well...
*/

#include <armadillo>

using namespace juce;

#include "Components/Component.h"
#include "Components/ComponentFactory.h"
#include "Model/Model.h"
