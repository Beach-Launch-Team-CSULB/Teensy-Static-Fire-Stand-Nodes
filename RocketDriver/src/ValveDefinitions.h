#ifndef VALVEDEFINITIONS_H
#define VALVEDEFINITIONS_H

#include "ValveClass.h"
#include <array>

// Define number of valves here
#define NUM_VALVES 16
#define NUM_VALVEENABLE 10

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
Valve LoxDomeReg{19, 3, NormalClosed, 5, 25000, false};
Valve LoxDomeRegVent{20, 3, NormalClosed, 6, 25000, false};
Valve FuelVent{21, 3, NormalClosed, 2, 25000, false};               //WRONG PIN - INSPECT WIRING, was set to 2 and changed for breadboard testing, (was now 30?)
Valve FuelDomeReg{22, 3, NormalClosed, 7, 25000, false};
Valve FuelDomeRegVent{23, 3, NormalClosed, 8, 25000, false};
// On Pasafire Node
Valve PasaLoxVent{18, 15, NormalOpen, 5, 500000, false, 166};        //18
Valve PasaLoxDomeReg{19, 15, NormalClosed, 6, 25000, false};           //19
Valve PasaFuelVent{21, 15, NormalClosed, 7, 25000, false};           //21
Valve PasaFuelDomeReg{22, 15, NormalClosed, 8, 25000, false};        //22
Valve PasaLoxMV{24, 15, NormalClosed, 9, 25000, false, -1, 50};      //24
Valve PasaFuelMV{25, 15, NormalClosed, 10, 25000, false, -1, 50};     //25

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
//std::array<Valve*, NUM_VALVES> valveArray{&HiPress, &HiPressVent, &LoxVent, &LoxDomeReg, &LoxDomeRegVent, &FuelVent, &FuelDomeReg, &FuelDomeRegVent, &FuelMV, &LoxMV};
std::array<Valve*, NUM_VALVES> valveArray{&HiPress, &HiPressVent, &LoxVent, &LoxDomeReg, &LoxDomeRegVent, &FuelVent, &FuelDomeReg, &FuelDomeRegVent, &FuelMV, &LoxMV, &PasaLoxVent, &PasaLoxDomeReg, &PasaFuelVent, &PasaFuelDomeReg, &PasaLoxMV, &PasaFuelMV};


// Declare all ValveEnable Objects here
// LISTED AS {ValveEnableID, Pin, ValveEnableNodeID}

// On Engine Node
ValveEnable HiPressHiVentSafe{32, 24, 2};
ValveEnable MainValvesSafe{37, 26, 2}; //was 25 but I think wired to 26
// On Prop Node
ValveEnable FuelVentSafe{33, 24, 3};
ValveEnable LoxDomeRegVentSafe{34, 25, 3};
ValveEnable FuelDomeRegVentSafe{35, 26, 3};
ValveEnable LoxVentSafe{36, 27, 3};
// On Pasafire Node
ValveEnable PasaFuelVentSafe{11, 11, 15};
ValveEnable PasaLoxVentSafe{12, 12, 15};
ValveEnable PasaMainValvesSafe{24, 24, 15};
ValveEnable PasaLoxFuelDomesSafe{25, 25, 15}; 

// BabyShark should be one to one Valve Enables to outputs with planned ALARA design

// ADD VALVE Enables TO THIS VALVE Enable ARRAY IN THE FORM: &VALVEENABLE
//std::array<ValveEnable*, NUM_VALVEENABLE> valveEnableArray{&HiPressHiVentSafe, &MainValvesSafe, &FuelVentSafe, &LoxDomeRegVentSafe, &FuelDomeRegVentSafe, &LoxVentSafe};
std::array<ValveEnable*, NUM_VALVEENABLE> valveEnableArray{&HiPressHiVentSafe, &MainValvesSafe, &FuelVentSafe, &LoxDomeRegVentSafe, &FuelDomeRegVentSafe, &LoxVentSafe, &PasaFuelVentSafe, &PasaLoxVentSafe, &PasaMainValvesSafe, &PasaLoxFuelDomesSafe};

#endif