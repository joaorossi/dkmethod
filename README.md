# DK-Method for JUCE
A JUCE module for analog emulation using the Discrete K-Method.

Authors: 
	João Rossi Filho - joaorossifilho@gmail.com
	Jaromir Macak    - jarda.macak@seznam.cz

---

# How to Use?
The code is formated as a JUCE for module, allowing easy integration with JUCE based applications.  
After cloning the repository, there are 2 options for adding the module to Projucer:
 - Adding the *dkmethod* directory to Projucer's *Global Path -> User Modules*
 - Specifying the *dkmethod* directly on *Module -> + -> Add module from specified folder...*

---

# Dependencies
JUCE (obviously...) and [Armadillo](https://gitlab.com/conradsnicta/armadillo-code)

## Mac OSX
If you're on Mac, then it should be easy. All you'll have to do is get Armadillo, and add its
include directory to *Header Search Paths* field. The module should automatically link it
to Accelerate framework.  
You won't even have to build Armadillo!  

## Windows
Suport for Windows isn't tested, but it should work fine, the main issue is that Armadillo relies on
[LAPACK](http://icl.cs.utk.edu/lapack-for-windows/) and [OpenBLAS](https://www.openblas.net/)
and it isn't trivial to build them for Windows, specially considering you should build them
for static linking, to allow easier distribuition.  
After building it (please let me know if you get there) you'll have to add Armadillo include search paths,
library search paths for LAPACK, OpenBLAS and Armadillo, and add them at *External Libraries to Link*.  
The goal is to ship pre-built LAPACK and OpenBLAS binaries with the module, until then
Windows users will have to build Armadillo on their own.

## Linux
Suport to Linux isn't tested, but should also be fine.  
Build or installing BLAS, LAPACK and Armadillo on Linux is quite trivial,
follow their [instructions](https://gitlab.com/conradsnicta/armadillo-code).  
After building and installing, add the Armadillo include and library search paths
and add the libraries at *External Libraries to Link*.

---

# Roadmap

 - Add variable resistor component.
 - Switch nonlinear solver to *Damped Newton-Raphson*
 - Employ pre-calculated Lookup-Tables to increase efficiecy.
 - Add Windows support (include pre-built BLAS and LAPACK binaries).

---

# Acknowledgements

 - Nodal DK Framework, by Jaromir Macak. [Repo](https://github.com/jardamacak/NodalDKFramework)
 - M. Holters, U. Zölzer – Physical Modelling of a Wah-wah Effect Pedal as a Case Study for Application of the Nodal DK Method to Circuits with Variable Parts, Proc. of the 14th International Conference on Digital Audio Effects (DAFx-11), Paris, France, September 19-23, 2011
 - C. Sanderson, R. Curtin. - A User-Friendly Hybrid Sparse Matrix Class in C++. International Congress on Mathematical Software, 2018

 ---

# License

This code is under GPL3 license and everybody is encoureged to use it!  
For comercial licensing inqueries, please contact the authors.
