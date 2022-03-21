#ifndef CONTROLFUNCTIONS_H
#define CONTROLFUNCTIONS_H

#include "StateList.h"
#include "ControlCommands.h"
#include "ValveClass.h"
#include "PyroClass.h"
#include "AutoSequenceClass.h"
#include "SensorClass.h"
#include <array>

//importing these for the NUM_ defines and passing into ControlFunctions.cpp
/* #include "ValveDefinitions.h"
#include "PyroDefinitions.h"
#include "AutoSequenceDefinitions.h" */


#define NUM_VALVES 10
#define NUM_VALVEENABLE 6
#define NUM_PYROS 2
#define NUM_AUTOSEQUENCES 1
#define NUM_SENSORS 17


/* 
//Pasafire Define Numbers Only Here - Uncomment these and comment out BLT ones
#define NUM_VALVES 6
#define NUM_VALVEENABLE 4
 */

// checks the state that was set at start-up and issues the appropriate command as current command
void startupStateCheck(const State& currentState, Command& currentCommand);

// takes the current command and executes the necessary instructions
void commandExecute(State& currentState, State& priorState, Command& currentCommand, const std::array<Valve*, NUM_VALVES>& valveArray, const std::array<Pyro*, NUM_PYROS>& pyroArray, const std::array<ValveEnable*, NUM_VALVEENABLE>& valveEnableArray, const std::array<AutoSequence*, NUM_AUTOSEQUENCES>& autoSequenceArray, const std::array<MCU_SENSOR*, NUM_SENSORS>& sensorArray, bool &HaltFlag);

// Used with Abort to override control of main valves and engine igniters

void haltFlagCheck(bool &haltFlag, const std::array<Valve*, NUM_VALVES>& valveArray, const std::array<Pyro*, NUM_PYROS>& pyroArray, const std::array<ValveEnable*, NUM_VALVEENABLE>& valveEnableArray);



#endif