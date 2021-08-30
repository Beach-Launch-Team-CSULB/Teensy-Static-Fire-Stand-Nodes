#ifndef SENSORDEFINITIONS_H
#define SENSORDEFINITIONS_H

#include "SensorClass.h"
#include <array>
#include <ADC.h>

// define number of sensors here
#define NUM_SENSORS 11

// initialize all sensor objects here (PINS NOT CORRECT)
SENSOR pt0 {0,"ChamberPT1", pt1000, 0, 500, 16};
SENSOR pt1 {1,"FuelTankPT", pt1000, 23, 500, 16};
SENSOR pt2 {2, "LOXTankPT", pt1000, 24, 500, 16};
SENSOR pt3 {3, "FuelRegPT", pt1000, 21, 500, 16};
SENSOR pt4 {4, "LOXRegPT", pt1000, 22, 500, 16};
SENSOR pt5 {5, "FuelLinePT", pt1000, 25, 500, 16};
SENSOR pt6 {6, "LOXLinePT", pt1000, 26, 500, 16};
SENSOR pt7 {7, "PneumaticPT", pt1000, 27, 500, 16};
SENSOR load_cell1 {8, "ThrustMountLoadCell1", loadcell1000, 1, 100, 16};


// place sensor objects into this array
std::array<SENSOR*, NUM_SENSORS> sensorArray{&pt1, &pt2, &pt3, &pt4, &pt5, &pt6, &pt7, &load_cell1};




#endif