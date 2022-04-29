#ifndef PyroDefinitions_H
#define PyroDefinitions_H

#include "PyroClass.h"
#include <array>

//Igniter outputs - setting up almost the same as valves but no PWM
#define NUM_PYROS 4

Pyro EngineIgniter0{26, 2, Solo, 31, 2000000, false, -4000000}; // was firepin 11
Pyro EngineIgniter1{27, 2, Solo, 32, 2000000, false, -4000000}; // was firepin 12

//Pasafire Versions - Same setup but different node/settings possibly
Pyro PasaEngineIgniter0{26, 15, Solo, 31, 2000000, false, -4000000};
Pyro PasaEngineIgniter1{27, 15, Solo, 32, 2000000, false, -4000000};


// ADD PYROS TO THIS VALVE ARRAY IN THE FORM: &PYRO
//std::array<Pyro*, NUM_PYROS> pyroArray{&EngineIgniter0, &EngineIgniter1};
std::array<Pyro*, NUM_PYROS> pyroArray{&EngineIgniter0, &EngineIgniter1, &PasaEngineIgniter0, &PasaEngineIgniter1};

#endif