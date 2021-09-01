#ifndef CONTROLFUNCTIONS_H
#define CONTROLFUNCTIONS_H

#include "StateList.h"
#include "ControlCommands.h"
#include "ValveClass.h"
#include <array>

#define NUM_VALVES 6

// checks the state that was set at start-up and issues the appropriate command as current command
void startupStateCheck(const State& currentState, Command& currentCommand);

// takes the current command and executes the necessary instructions
void commandExecute(State& currentState, Command& currentCommand, const std::array<Valve*, NUM_VALVES>& valveArray);

#endif