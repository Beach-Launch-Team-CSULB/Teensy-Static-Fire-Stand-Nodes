#ifndef SENSORDEFINITIONS_H
#define SENSORDEFINITIONS_H

#include "SensorClass.h"
#include <array>
#include <ADC.h>


// define number of sensors here
#define NUM_SENSORS 17

// initialize all sensor objects here
// Renegade SF Stand
MCU_SENSOR ThrustMountLoadCell1{50, 2, A0, 2, 10, 150, false};
MCU_SENSOR ThrustMountLoadCell2{51, 2, A1, 2, 10, 150, false};
MCU_SENSOR ThrustMountLoadCell3{52, 2, A2, 2, 10, 150, false};
MCU_SENSOR ChamberPT2{55, 2, A5, 100, 100, 1000, false};
MCU_SENSOR ChamberPT1{56, 2, A6, 10, 100, 1000, false};
MCU_SENSOR FuelInletPropSidePT{57, 2, A7, 2, 10, 100, false};
MCU_SENSOR FuelInjectorPT{58, 2, A8, 10, 100, 1000, false};
MCU_SENSOR LoxInletPropSidePT{59, 2, A9, 2, 10, 100, false};
MCU_SENSOR MVPneumaticsPT{78, 3, A3, 2, 10, 100, false};
MCU_SENSOR DomeRegFuelPT{79, 3, A4, 2, 10, 100, false};
MCU_SENSOR DomeRegLoxPT{80, 3, A5, 2, 10, 100, false};
MCU_SENSOR FuelTankPT{81, 3, A6, 2, 10, 100, false};
MCU_SENSOR LoxTankPT{82, 3, A7, 2, 10, 100, false};
MCU_SENSOR HiPressFuelPT{83, 3, A8, 2, 10, 100, false};
MCU_SENSOR HiPressLoxPT{84, 3, A9, 2, 10, 100, false};
MCU_SENSOR MCUtempNode2 {200, 2, 70, 2, 10, 100, true};
MCU_SENSOR MCUtempNode3 {300, 3, 70, 2, 10, 100, true};

std::array<MCU_SENSOR*, NUM_SENSORS> sensorArray{&ThrustMountLoadCell1, &ThrustMountLoadCell2, &ThrustMountLoadCell3, &ChamberPT2, &ChamberPT1, &FuelInletPropSidePT, &FuelInjectorPT, &LoxInletPropSidePT, &MVPneumaticsPT, &DomeRegFuelPT, &DomeRegLoxPT, &FuelTankPT, &LoxTankPT, &HiPressFuelPT, &HiPressLoxPT, &MCUtempNode2, &MCUtempNode3};





//Old Pasafire config
/* // initialize all sensor objects here (PINS NOT CORRECT)
SENSOR pt0{0, 2, ChamberPT1, TeensyMCUADC, pt,  A14, 500, 16};
SENSOR pt1{1, 3, FuelTankPT, TeensyMCUADC, pt,  A15, 500, 16};
SENSOR pt2{2, 3, LOXTankPT,TeensyMCUADC, pt,  A16, 500, 16};
SENSOR pt3{3, 3, FuelRegPT, TeensyMCUADC,pt,  A17, 500, 16};
SENSOR pt4{4, 3, LOXRegPT, TeensyMCUADC,pt,  A18, 500, 16};
SENSOR pt5{5, 2, FuelLinePT, TeensyMCUADC,pt,  A19, 500, 16};
SENSOR pt6{6, 2, LOXLinePT, TeensyMCUADC,pt,  A20, 500, 16};
SENSOR pt7{7, 3, PneumaticPT, TeensyMCUADC,pt,  A21, 500, 16};
SENSOR load_cell1{8, 2, ThrustMountLoadCell1, TeensyMCUADC,loadcell1000,  A22, 100, 16};


// place sensor objects into this array
std::array<SENSOR*, NUM_SENSORS> sensorArray{&pt1, &pt2, &pt3, &pt4, &pt5, &pt6, &pt7, &load_cell1};
 */



#endif