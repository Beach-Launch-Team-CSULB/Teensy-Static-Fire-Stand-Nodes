#ifndef VALVEDEFINITIONS_H
#define VALVEDEFINITIONS_H

#include "ValveClass.h"
#include <array>

// Define number of valves here
#define NUM_VALVES 6

// Declare all Valve Objects here using ValveClass, and add them to the valveArray
Valve loxVent{0, NormalOpen, 2, 500000, 0, 166};
Valve fuelVent{1, NormalClosed, 5, 25000};
Valve loxIso{2, NormalClosed, 6, 25000};
Valve fuelIso{3, NormalClosed, 7, 25000};
Valve loxMV{4, NormalClosed, 8, 25000};
Valve fuelMV{5, NormalClosed, 9, 25000};



// ADD VALVES TO THIS VALVE ARRAY IN THE FORM: &VALVE

std::array<Valve*, NUM_VALVES> valveArray{&loxVent, &fuelVent, &loxIso, &fuelIso, &loxMV, &fuelMV};

#endif