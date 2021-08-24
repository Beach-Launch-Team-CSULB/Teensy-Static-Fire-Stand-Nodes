#ifndef OPERATIONFUNCTIONS_H
#define OPERATIONFUNCTIONS_H

#define BITFLAG_SIZE 16

#include "ValveClass.h"
#include "SensorClass.h"
#include <array>
#include <bitset>
#include <FlexCAN.h>
#include <ADC.h>



// This contains some of the functions to be used during operations they are templates, and so defined in the header. BEWARE

//  CALL THIS FUNCTION EVERY LOOP 
    // This function takes the array of pointers that point to the valve objects, and then calls the .stateOperations() method for each valve
    // Make sure valveArray is an array of pointers, as defined in ValveDefinitions.h
template <typename T, std::size_t size>
void valveTasks(const std::array<T, size>& valveArray)
{
    // iterate through valve array and run the stateOperations method
    for(auto valve : valveArray)
    {
        valve->stateOperations();
        //Serial.print("LoopRan");
    }
}

// CALL THIS FUNCTION ONCE IN SETUP, THIS SETS THE VALVE PINMODES
    // make sure to pass this function valveArray, as defined in ValveDefinitions.h
template <typename T, std::size_t size>
void valveSetUp(const std::array<T, size>& valveArray)
{
    // iterate through valve array and run the stateOperations method
    for(auto valve : valveArray)
    {
        valve->begin();
        //Serial.print("LoopRan");
    }
}

// This function outputs a bitset contianing flags about the valve status, to be sent back to the console via CAN
template <typename T, std::size_t size>
std::bitset<BITFLAG_SIZE> setValveFlags(const std::array<T, size>& valveArray)
{
    std::bitset<BITFLAG_SIZE> valveStatus{0b0000'0000'0000'0000};


    for (auto valve : valveArray)
    {
        ValveState valveState{valve->getState()};
        if((valveState == ValveState::Open) || (valveState == ValveState::OpenProcess) || (valveState == ValveState::CloseCommanded)) // these two states count as open, everything else is closed
        {
            valveStatus.set(valve->getValveID());
            //Serial.println("Open");
        }
        else
        {
            //Serial.println("Closed");
        }

    }
    
    return valveStatus;
}

// this runs the begin method for each sensor
template <std::size_t size>
void sensorSetUp(const std::array<SENSOR*, size>& sensorArray)
{
    for(auto sensor : sensorArray)
    {
        sensor->begin();
    }
}

// This function reads all the sensor values
template <std::size_t size>
void readSensors(const std::array<SENSOR*, size>& sensorArray, ADC* adc)
{
    for(auto sensor : sensorArray)
    {
        sensor->read(adc);
    }
}


#endif