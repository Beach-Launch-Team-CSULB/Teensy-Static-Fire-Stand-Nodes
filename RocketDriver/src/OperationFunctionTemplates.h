#ifndef OPERATIONFUNCTIONS_H
#define OPERATIONFUNCTIONS_H

//#define BITFLAG_SIZE 16


#include <array>
#include <bitset>
#include <FlexCAN.h>
//#include <ADC.h>



// This contains some of the functions to be used during operations they are templates, and so defined in the header. BEWARE

//  CALL THIS FUNCTION EVERY LOOP 
    // This function takes the array of pointers that point to the valve objects, and then calls the .stateOperations() method for each valve
    // Make sure valveArray is an array of pointers, as defined in ValveDefinitions.h
template <typename T, std::size_t size>
void valveTasks(const std::array<T, size>& valveArray, uint8_t& nodeIDReadIn)
{
    // iterate through valve array and run the stateOperations method
    for(auto valve : valveArray)
    {
    
        if (valve->getValveNodeID() == nodeIDReadIn)
            {
            valve->stateOperations();
            //Serial.print("LoopRan");
            }
    }
}

template <typename T, std::size_t size>
void valveEnableTasks(const std::array<T, size>& valveEnableArray, uint8_t& nodeIDReadIn)
{
    // iterate through valve array and run the stateOperations method
    for(auto valveEnable : valveEnableArray)
    {
        if (valveEnable->getValveEnableNodeID() == nodeIDReadIn)
        {  
            if (valveEnable->getState() != (valveEnable->getPriorState()))
            {
                //Serial.print("sup");
                //Serial.print(valveEnable->getValveEnableID());
                //Serial.print("LoopRan");
                valveEnable->stateOperations();
            }
        }
    }
}

template <typename T, std::size_t size>
void pyroTasks(const std::array<T, size>& pyroArray, uint8_t& nodeIDReadIn)
{
    // iterate through valve array and run the stateOperations method
    for(auto pyro : pyroArray)
    {
        
    if (pyro->getPyroNodeID() == nodeIDReadIn)
            {
            pyro->stateOperations();
            //Serial.print("LoopRan");
            }
    }
}

template <typename T, std::size_t size>
void autoSequenceTasks(const std::array<T, size>& autoSequenceArray, uint8_t& nodeIDReadIn)
{
    // iterate through valve array and run the stateOperations method
    for(auto autoSequence : autoSequenceArray)
    {
        
    if (autoSequence->getHostNodeID() == nodeIDReadIn)
            {
            autoSequence->stateOperations();
            //Serial.print("LoopRan");
            }
    }
}

template <typename T, std::size_t size>
void sensorTasks(const std::array<T, size>& sensorArray, ADC*adc, uint8_t& nodeIDReadIn)
{
    // iterate through valve array and run the stateOperations method
    for(auto sensor : sensorArray)
    {
    
        if (sensor->getSensorNodeID() == nodeIDReadIn)
            {
            sensor->stateOperations();
            //Serial.print("LoopRan");
            sensor->read(adc);
            // Could add simple CAN send here, but should be a later function for more flexibility
            sensor->linearConversion();
            }
        else if (nodeIDReadIn == 6)
        {
            sensor->linearConversion();
        }
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

template <typename T, std::size_t size>
void valveEnableSetUp(const std::array<T, size>& valveEnableArray)
{
    // iterate through valve array and run the stateOperations method
    for(auto valveEnable : valveEnableArray)
    {
        valveEnable->begin();
        //Serial.print("LoopRan");
    }
}

template <typename T, std::size_t size>
void pyroSetUp(const std::array<T, size>& pyroArray)
{
    // iterate through valve array and run the stateOperations method
    for(auto pyro : pyroArray)
    {
        pyro->begin();
        //Serial.print("LoopRan");
    }
}

template <typename T, std::size_t size>
void autoSequenceSetUp(const std::array<T, size>& autoSequenceArray)
{
    // iterate through valve array and run the stateOperations method
    for(auto autoSequence : autoSequenceArray)
    {
        autoSequence->begin();
        //Serial.print("LoopRan");
    }
}

template <typename T, std::size_t size>
void sensorSetUp(const std::array<T, size>& sensorArray)
{
    // iterate through valve array and run the stateOperations method
    for(auto sensor : sensorArray)
    {
        sensor->begin();
        //Serial.print("LoopRan");
    }
}



template <typename T, std::size_t size>
void autoSequenceValveUpdate(const std::array<T, size>& valveArray, int64_t& fireCurrentCountdown)
{
    // iterate through valve array and run the stateOperations method
    for (auto valve : valveArray)
    {
        if (valve->getState() != (ValveState::FireCommanded))
        {
            valve->setFireCommandBool(false);
        }        
        if (fireCurrentCountdown >= valve->getFireSequenceTime())
        {
            valve->setFireCommandBool(true);
        }    
    }
}

template <typename T, std::size_t size>
void autoSequencePyroUpdate(const std::array<T, size>& pyroArray, int64_t& fireCurrentCountdown)
{
    // iterate through pyro array and run the stateOperations method
    for(auto pyro : pyroArray)
    {
        if (pyro->getState() != (PyroState::FireCommanded))
        {
            pyro->setFireCommandBool(false);
        }
        if (fireCurrentCountdown >= pyro->getFireSequenceTime())
        {
            pyro->setFireCommandBool(true);
        }
        
    }

}

template <typename T, std::size_t size>
void ValveNodeIDCheck(const std::array<T, size>& valveArray, uint8_t nodeIDfromMain)
{
    // iterate through valve array and run the stateOperations method
    for (auto valve : valveArray)
    {
        if (valve->getValveNodeID() == nodeIDfromMain)
        {
            valve->setNodeIDCheck(true);
        }
    }
}

template <typename T, std::size_t size>
void ValveEnableNodeIDCheck(const std::array<T, size>& valveEnableArray, uint8_t nodeIDfromMain)
{
    // iterate through valveEnable array and run the stateOperations method
    for (auto valveEnable : valveEnableArray)
    {
        if (valveEnable->getValveEnableNodeID() == nodeIDfromMain)
        {
            valveEnable->setNodeIDCheck(true);
        }
    }
}

template <typename T, std::size_t size>
void PyroNodeIDCheck(const std::array<T, size>& pyroArray, uint8_t nodeIDfromMain)
{
    // iterate through pyro array and run the stateOperations method
    for (auto pyro : pyroArray)
    {
        if (pyro->getPyroNodeID() == nodeIDfromMain)
        {
            pyro->setNodeIDCheck(true);
        }
    }
}

template <typename T, std::size_t size>
void SensorNodeIDCheck(const std::array<T, size>& sensorArray, uint8_t nodeIDfromMain)
{
    // iterate through sensor array and run the stateOperations method
    for (auto sensor : sensorArray)
    {
        if (sensor->getSensorNodeID() == nodeIDfromMain)
        {
            sensor->setNodeIDCheck(true);
        }
        else if (nodeIDfromMain == 6)    //Logger nodeID so it generates array for all sensors
        {
            sensor->setNodeIDCheck(true);
        }
    }
}


/* // This function outputs a bitset contianing flags about the valve status, to be sent back to the console via CAN
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
} */

#endif