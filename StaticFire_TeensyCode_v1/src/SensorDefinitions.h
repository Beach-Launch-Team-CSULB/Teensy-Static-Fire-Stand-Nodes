#ifndef SENSORDEFINITIONS_H
#define SENSORDEFINITIONS_H

#include "SensorClass.h"
#include <array>

// define number of sensors here
#define NUM_SENSORS 11

// initialize all sensor objects here
SENSOR pt1 {"DomeRegFuel", pt1000, 21, 10};
SENSOR pt2 {"DomeRegLOX", pt1000, 22, 10};
SENSOR pt3 {"FuelTank", pt1000, 23, 100};
SENSOR pt4 {"LOXTank", pt1000, 24, 100};
SENSOR pt5 {"HiPress", pt5000, 20, 100};
SENSOR pt6 {"Chamber", pt1000, 0, 1000};
SENSOR load_cell1 {"Thrust Load Cell 1", loadcell1000, 1, 100};
SENSOR load_cell2 {"Thrust Load Cell 2", loadcell1000, 2, 100};
SENSOR load_cell3 {"Thrust Load Cell 3", loadcell1000, 3, 100};
SENSOR tc1 {"External Chamber Wall TC", tcktype,10,25};
SENSOR tc2 {"LOX Tank Lower TC", tcktype,12,1};


// place sensor objects into this array
std::array<SENSOR*, NUM_SENSORS> sensorArray{&pt1, &pt2, &pt3, &pt4, &pt5, &pt6, &load_cell1, &load_cell2, &load_cell3, &tc1, &tc2};




#endif