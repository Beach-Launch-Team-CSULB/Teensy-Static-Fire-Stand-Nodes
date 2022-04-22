#ifndef SENSORDEFINITIONS_H
#define SENSORDEFINITIONS_H

#include "SensorClass.h"
#include <array>

// define number of sensors here
// Renegade SF
//#define NUM_SENSORS 17
// Renegade SF and Pasafire SF
#define NUM_SENSORS 27

// initialize all sensor objects here
// Renegade SF Stand
MCU_SENSOR ThrustMountLoadCell1{50, 2, A0, 2, 10, 150, false};                     // 0
MCU_SENSOR ThrustMountLoadCell2{51, 2, A1, 2, 10, 150, false};                     // 1
MCU_SENSOR ThrustMountLoadCell3{52, 2, A2, 2, 10, 150, false};                     // 2
MCU_SENSOR ChamberPT2{55, 2, A5, 10, 100, 1000, false, 0.0186, -102.94};             // 3
MCU_SENSOR ChamberPT1{56, 2, A6, 10, 100, 1000, false, 0.0185, -128.88};             // 4
MCU_SENSOR FuelInletPropSidePT{57, 2, A7, 2, 10, 100, false, 0.0185, -125.74};       // 5
MCU_SENSOR FuelInjectorPT{58, 2, A8, 10, 100, 1000, false, 0.0186, -123.27};         // 6
MCU_SENSOR LoxInletPropSidePT{59, 2, A9, 2, 10, 100, false, 0.0186, -128.58};        // 7
MCU_SENSOR MVPneumaticsPT{78, 3, A3, 2, 10, 100, false, 0.0186, -126.56};            // 8
MCU_SENSOR DomeRegFuelPT{79, 3, A4, 2, 10, 100, false, 0.0186, -126.67};             // 9
MCU_SENSOR DomeRegLoxPT{80, 3, A5, 2, 10, 100, false, 0.0185, -133.36};              // 10
MCU_SENSOR FuelTankPT{81, 3, A6, 2, 10, 100, false, 0.0186, -129.3};                // 11
MCU_SENSOR LoxTankPT{82, 3, A7, 2, 10, 100, false, 0.0187, -125.36};                 // 12
MCU_SENSOR HiPressFuelPT{83, 3, A8, 2, 10, 100, false, 0.0933, -638.38};             // 13
MCU_SENSOR HiPressLoxPT{84, 3, A9, 2, 10, 100, false, 0.093, -629.72};              // 14
MCU_SENSOR MCUtempNode2{200, 2, 70, 2, 10, 100, true};                              // 15
MCU_SENSOR MCUtempNode3{300, 3, 70, 2, 10, 100, true};                              // 16

// initialize all sensor objects here (PINS NOT CORRECT?, calibrations definitely not correct)
// Pasafire config
MCU_SENSOR PasafireChamberPT1{214, 8, A14, 10, 100, 1000, false, 0.0185, -128.88};      // 17
MCU_SENSOR PasafireFuelTankPT{215, 8, A15, 2, 10, 100, false, 0.0186, -126.67};         // 18
MCU_SENSOR PasafireLOXTankPT{216, 8, A16, 2, 10, 100, false, 0.0186, -126.67};          // 19
MCU_SENSOR PasafireFuelRegPT{217, 8, A17, 2, 10, 100, false, 0.0186, -126.67};          // 20
MCU_SENSOR PasafireLOXRegPT{218, 8, A18, 2, 10, 100, false, 0.0186, -126.67};           // 21
MCU_SENSOR PasafireFuelLinePT{219, 8, A19, 2, 10, 100, false, 0.0186, -126.67};         // 22
MCU_SENSOR PasafireLOXLinePT{220, 8, A20, 2, 10, 100, false, 0.0186, -126.67};          // 23
MCU_SENSOR PasafirePneumaticPT{221, 8, A21, 2, 10, 100, false, 0.0186, -126.56};        // 24
MCU_SENSOR PasafireThrustMountLoadCell1{222, 8, A22, 2, 10, 150, false};                // 25
MCU_SENSOR MCUtempNode8{800, 8, 70, 2, 10, 100, true};                                  // 26

// Sensor Array including Renegade SF only
//std::array<MCU_SENSOR*, NUM_SENSORS> sensorArray{&ThrustMountLoadCell1, &ThrustMountLoadCell2, &ThrustMountLoadCell3, &ChamberPT2, &ChamberPT1, &FuelInletPropSidePT, &FuelInjectorPT, &LoxInletPropSidePT, &MVPneumaticsPT, &DomeRegFuelPT, &DomeRegLoxPT, &FuelTankPT, &LoxTankPT, &HiPressFuelPT, &HiPressLoxPT, &MCUtempNode2, &MCUtempNode3};
// Sensor Array including Renegade SF and Pasafire SF
std::array<MCU_SENSOR*, NUM_SENSORS> sensorArray{&ThrustMountLoadCell1, &ThrustMountLoadCell2, &ThrustMountLoadCell3, &ChamberPT2, &ChamberPT1, &FuelInletPropSidePT, &FuelInjectorPT, &LoxInletPropSidePT, &MVPneumaticsPT, &DomeRegFuelPT, &DomeRegLoxPT, &FuelTankPT, &LoxTankPT, &HiPressFuelPT, &HiPressLoxPT, &MCUtempNode2, &MCUtempNode3, &PasafireChamberPT1, &PasafireFuelTankPT, &PasafireLOXTankPT, &PasafireFuelRegPT, &PasafireLOXRegPT, &PasafireFuelLinePT, &PasafireLOXLinePT, &PasafirePneumaticPT, &PasafireThrustMountLoadCell1, &MCUtempNode8};

#endif