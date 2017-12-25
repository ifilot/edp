# EDP

## Introduction
Electron density plotter is a tiny tool for generating electron density plots from VASP output.

## Compilation
EDP depends on a couple of libraries, which are normally directly available by your favorite package manager.
* Boost
* GLM
* Cairo
* TCLAP

Compilation is done using CMake
```
mkdir build
cd build
cmake ../src
make -j5
```

## Usage
A short tutorial on using the program is provided in this [blog post](http://www.ivofilot.nl/posts/view/27/Visualising+the+electron+density+of+the+binding+orbitals+of+the+CO+molecule+using+VASP).
