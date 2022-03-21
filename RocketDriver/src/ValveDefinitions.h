#ifndef VALVEDEFINITIONS_H
#define VALVEDEFINITIONS_H

#include "ValveClass.h"
#include <array>

// Define number of valves here
#define NUM_VALVES 10
#define NUM_VALVEENABLE 6

// Declare all Valve Objects here using ValveClass, and add them to the valveArray
// On Renegade SF Engine Node
/* Valve HiPress{16, 2, NormalClosed, 2, 25000, false};
Valve HiPressVent{17, 2, NormalClosed, 5, 25000, false};
Valve LoxMV{24, 2, NormalClosed, 7, 25000, false, -1, 50};
Valve FuelMV{25, 2, NormalClosed, 6, 25000, false, -1, 50};
// On Renegade SF Prop Node
Valve LoxVent{18, 3, NormalOpen, 2, 500000, false, 166};
Valve LoxDomeReg{19, 3, NormalClosed, 5, 25000, false};
Valve LoxDomeRegVent{20, 3, NormalClosed, 6, 25000, false};
Valve FuelVent{21, 3, NormalClosed, 2, 25000, false};               //WRONG PIN - INSPECT WIRING, was set to 2 and changed for breadboard testing
Valve FuelDomeReg{22, 3, NormalClosed, 7, 25000, false};
Valve FuelDomeRegVent{23, 3, NormalClosed, 8, 25000, false}; */

// On Renegade SF Engine Node
Valve HiPress{16, 2, NormalClosed, 2, 25000, false};
Valve HiPressVent{17, 2, NormalClosed, 5, 25000, false};
Valve LoxMV{24, 2, NormalClosed, 7, 25000, false, -1, 50};
Valve FuelMV{25, 2, NormalClosed, 6, 25000, false, -1, 50};
// On Renegade SF Prop Node
Valve LoxVent{18, 3, NormalOpen, 9, 500000, false, 166};
Valve LoxDomeReg{19, 3, NormalClosed, 10, 25000, false};
Valve LoxDomeRegVent{20, 3, NormalClosed, 29, 25000, false};
Valve FuelVent{21, 3, NormalClosed, 30, 25000, false};               //WRONG PIN - INSPECT WIRING, was set to 2 and changed for breadboard testing
Valve FuelDomeReg{22, 3, NormalClosed, 38, 25000, false};
Valve FuelDomeRegVent{23, 3, NormalClosed, 8, 25000, false};





// On Pasafire Node
/* Valve PasaLoxVent{18, 8, NormalOpen, 2, 500000, false, 166};
Valve PasaLoxDomeReg{19, 8, NormalOpen, 5, 25000, false};
Valve PasaFuelVent{21, 8, NormalClosed, 2, 25000, false};
Valve PasaFuelDomeReg{22, 8, NormalClosed, 7, 25000, false};
Valve PasaLoxMV{24, 8, NormalClosed, 7, 25000, false, -1, 50};
Valve PasaFuelMV{25, 8, NormalClosed, 6, 25000, false, -1, 50}; */
// On BabyShark Pad Node
/* Valve HiPress{16, 2, NormalClosed, 2, 25000, false};
Valve HiPressVent{17, 2, NormalClosed, 5, 25000, false};
Valve LoxMV{24, 2, NormalClosed, 7, 25000, false, -1, 50};
Valve FuelMV{25, 2, NormalClosed, 6, 25000, false, -1, 50}; */
// On BabyShark Propulsion Node
/* Valve LoxVent{18, 3, NormalOpen, 2, 500000, false, 166};
Valve CommonDomeReg{19, 3, NormalClosed, 5, 25000, false};
Valve CommonDomeRegVent{20, 3, NormalClosed, 6, 25000, false};
Valve FuelVent{21, 3, NormalClosed, 2, 25000, false};
Valve CommonDomeThrottle{23, 3, NormalClosed, 8, 25000, false}; */


// ADD VALVES TO THIS VALVE ARRAY IN THE FORM: &VALVE
std::array<Valve*, NUM_VALVES> valveArray{&HiPress, &HiPressVent, &LoxVent, &LoxDomeReg, &LoxDomeRegVent, &FuelVent, &FuelDomeReg, &FuelDomeRegVent, &FuelMV, &LoxMV};
//std::array<Valve*, NUM_VALVES> valveArray{&HiPress, &HiPressVent, &LoxVent, &LoxDomeReg, &LoxDomeRegVent, &FuelVent, &FuelDomeReg, &FuelDomeRegVent, &FuelMV, &LoxMV, &PasaLoxVent, &PasaLoxDomeReg, &PasaFuelVent, &PasaFuelDomeReg, &PasaLoxMV, &PasaFuelMV};


// Declare all ValveEnable Objects here
// THESE ARE IN THE WRONG ORDER AND WE KNOW THAT, WEIRD ISSUES WITH INSTANTIATING VALVE ENABLES INCORRECTLY
// LISTED AS {ValveEnableID, Pin, ValveEnableNodeID}

// On Engine Node
ValveEnable HiPressHiVentSafe{32, 24, 2};
ValveEnable MainValvesSafe{37, 25, 2};
// On Prop Node
ValveEnable FuelVentSafe{33, 24, 3};
ValveEnable LoxDomeRegVentSafe{34, 25, 3};
ValveEnable FuelDomeRegVentSafe{35, 26, 3};
ValveEnable LoxVentSafe{36, 27, 3};
// On Pasafire Node
/* ValveEnable PasaFuelVentSafe{33, 24, 8};
ValveEnable PasaLoxVentSafe{36, 27, 8};
ValveEnable PasaMainValvesSafe{37, 25, 8};
ValveEnable PasaLoxFuelDomesSafe{38, 25, 8};  */

// BabyShark should be one to one Valve Enables to outputs with planned ALARA design

// ADD VALVE Enables TO THIS VALVE Enable ARRAY IN THE FORM: &VALVEENABLE
std::array<ValveEnable*, NUM_VALVEENABLE> valveEnableArray{&HiPressHiVentSafe, &MainValvesSafe, &FuelVentSafe, &LoxDomeRegVentSafe, &FuelDomeRegVentSafe, &LoxVentSafe};
//std::array<ValveEnable*, NUM_VALVEENABLE> valveEnableArray{&HiPressHiVentSafe, &MainValvesSafe, &FuelVentSafe, &LoxDomeRegVentSafe, &FuelDomeRegVentSafe, &LoxVentSafe, &PasaFuelVentSafe, &PasaLoxVentSafe, &PasaMainValvesSafe, &PasaLoxFuelDomesSafe};

#endif