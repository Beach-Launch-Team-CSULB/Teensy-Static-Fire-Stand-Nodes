#ifndef VALVEDEFINITIONS_H
#define VALVEDEFINITIONS_H

#include "ValveClass.h"
#include <array>

// Define number of valves here
#define NUM_VALVES 1

// Declare all Valve Objects here using ValveClass, and add them to the valveArray
Valve testValve{0, NormalOpen, 2, 500000, 0, 166};



// ADD VALVES TO THIS VALVE ARRAY IN THE FORM: &VALVE

std::array<Valve*, NUM_VALVES> valveArray{&testValve};

#endif