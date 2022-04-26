#ifndef CANREPORTS_H
#define CANREPORTS_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <array>

#include "CANWrite.h"
#include "StateList.h"
#include "ControlFunctions.h"
#include <vector>

uint8_t engineNodeValveNum = 4;
uint8_t propNodeValveNum = 6;
uint8_t PasafireNodeValveNum = 6;
uint8_t engineNodePyroNum = 2;
uint8_t propNodePyroNum = 0;
uint8_t PasafireNodePyroNum = 2;

uint8_t vavleArrayCount;
uint8_t pyroArrayCount;

elapsedMicros PropSysReportTimer;
elapsedMicros AutoSequenceReportTimer;
elapsedMicros ValveReportTimer;


// General Level State Report - covers overall state of whole node
void CAN2PropSystemStateReport(FlexCAN& CANbus, State& currentState, Command& currentCommand, const std::array<Valve*, NUM_VALVES>& valveArray, const std::array<Pyro*, NUM_PYROS>& pyroArray, const std::array<ValveEnable*, NUM_VALVEENABLE>& valveEnableArray, bool & haltFlag, uint8_t nodeID)
{
if (PropSysReportTimer >= 1000000)
{
    //Serial.println(now());
    //Hardcoded array iterator sizes because I'm not smart enough to fix the auto arrays yet
    if (nodeID == 2)
    {
        vavleArrayCount = engineNodeValveNum;
        pyroArrayCount = engineNodePyroNum;
    }
    else if (nodeID == 3)
    {
        vavleArrayCount = propNodeValveNum;
        pyroArrayCount = propNodePyroNum;
    }
    else if (nodeID == 15)
    {
        vavleArrayCount = PasafireNodeValveNum;
        pyroArrayCount = PasafireNodePyroNum;
    }    
    else
    {
        vavleArrayCount = 7;
        pyroArrayCount = 0;
    }


    // build message
        static CAN_message_t msgOut1;
        static CAN_message_t msgOut2;
        msgOut1.ext = 0;
        msgOut2.ext = 0;
        msgOut1.id = nodeID;            //Make this more specific later
        msgOut2.id = nodeID + 16;       //Make this more specific later

        // CAN BYTE - System state and Valve Safety Enable States
        int8_t currentStateEnumToInt = static_cast<int8_t>(currentState);
        //Serial.print("static cast of StateEnumToInt: ");
        //Serial.println(currentStateEnumToInt);
        bool ValveSafetyEnableStateArray[4] ={};
        uint8_t ShiftedValveSafetyEnableStateArray = 0;
        uint8_t EnableArrayBit = 0;    //for the valve enable iterator

        // iterate through valveEnable array and get states for state report
        for(auto valveEnable : valveEnableArray)
        {
            if (valveEnable->getValveEnableNodeID() == nodeID)
            {    
                //for (size_t i = 0; i < 3; i++) //I need to add something for sizes less than 3 so it doesn't bug out
                //{
                bool ValveEnableEnumToBool = static_cast<bool>(valveEnable->getState());
                ValveSafetyEnableStateArray[EnableArrayBit] = ValveEnableEnumToBool;
                EnableArrayBit++;
                //}
            }        
        }

        //bitshifting array
        for (size_t i = 0; i < 4; i++)
        {
        uint8_t bitShiftNumber = 4 + i;
        uint8_t loopyboi = (ValveSafetyEnableStateArray[i])<<bitShiftNumber;
        ShiftedValveSafetyEnableStateArray = ShiftedValveSafetyEnableStateArray + loopyboi;
        }

        //valveEnableArray;
        msgOut1.buf[0] = currentStateEnumToInt + ShiftedValveSafetyEnableStateArray;
        //msgOut.buf[0] = 111;

        //Valve State information bytes
        uint8_t canByte = 1;    //starts the valve state bytes skipping 1 byte(s) first
            //to limit overflow of CAN2.0 max message bytes
        
        //while (canByte <=7)
        //{
            //while (canByte <=vavleArrayCount) //HARDCODING number of valves on engine node so it doesn't loop
            //{
            
    /*         // iterate through valve array - attempting to do it different way
            for (valveArray)
            {

            } */
            
            
            // iterate through valve array 
            for(auto valve : valveArray)
            {
                
                    if (valve->getValveNodeID() == nodeID)
                    {
                    uint8_t valveID = static_cast<uint8_t>(valve->getValveID());    
                    uint8_t ValveStateEnumToInt = static_cast<uint8_t>(valve->getState());
                    uint8_t ShiftedValveStateEnumToInt = (ValveStateEnumToInt<<5);
                    
                    if (canByte <= 7)
                    {
                    msgOut1.buf[canByte] = valveID + ShiftedValveStateEnumToInt;
                    }
                    else if (canByte <= 14&&canByte > 7)
                    {
                    msgOut2.buf[canByte-7] = valveID + ShiftedValveStateEnumToInt;
                    }
                    
                    //msgOut1.buf[canByte] = valveID + ShiftedValveStateEnumToInt;
                    Serial.print("ValveID: ");
                    Serial.print(valveID);
                    Serial.print( ": ValveState: ");
                    Serial.print(ValveStateEnumToInt);
                    Serial.print(": ");
                    //Serial.print(ShiftedValveStateEnumToInt);
                    //Serial.print("CANbyte: ");
                    Serial.println(canByte);
                    canByte++;
                    }
                //canByte++;
                
            }
            //}
            //while (canByte <=pyroArrayCount) //HARDCODING number of valves on engine node so it doesn't loop
            //{
            for(auto pyro : pyroArray)
            {
                    if (pyro->getPyroNodeID() == nodeID)
                    {
                    uint8_t pyroID = static_cast<uint8_t>(pyro->getPyroID());    
                    uint8_t PyroStateEnumToInt = static_cast<uint8_t>(pyro->getState());
                    uint8_t ShiftedPyroStateEnumToInt = (PyroStateEnumToInt<<5);

                    if (canByte <= 7)
                    {
                    msgOut1.buf[canByte] = pyroID + ShiftedPyroStateEnumToInt;
                    }
                    else if (canByte <= 14&&canByte > 7)
                    {
                    msgOut2.buf[canByte-7] = pyroID + ShiftedPyroStateEnumToInt;
                    }

                    //msgOut1.buf[canByte] = pyroID + ShiftedPyroStateEnumToInt;
                    Serial.print("PyroID: ");
                    Serial.print(pyroID);
                    Serial.print( ": PyroState: ");
                    Serial.print(PyroStateEnumToInt);
                    Serial.print(": ");
                    Serial.println(canByte);
                    canByte++;
                    }
            }
            //}
        //}
        
        // write message to bus
        Serial.print("ID: ");
        Serial.print(msgOut1.id);
        Serial.print(": ");
        for (size_t ii = 0; ii < 8; ii++)
        {
            Serial.print(msgOut1.buf[ii]);
            Serial.print(": ");
        }        Serial.print("ID: ");
        Serial.print(msgOut2.id);
        Serial.print(": ");
        for (size_t ii = 0; ii < 8; ii++)
        {
            Serial.print(msgOut2.buf[ii]);
            Serial.print(": ");
        }
        
        Serial.println();
        msgOut1.len = 8;
        msgOut2.len = 2;
        CANbus.write(msgOut1);
        CANbus.write(msgOut2);
        canByte = 1;
        
        PropSysReportTimer = 0;

    {
        // add write error handling here, for now it does nothing
    }
    }
//return ;
}

void CAN2AutosequenceTimerReport(FlexCAN& CANbus, const std::array<AutoSequence*, NUM_AUTOSEQUENCES>& autoSequenceArray, bool & haltFlag, int nodeID)
{
    if (AutoSequenceReportTimer >= 1000000)
    {
    // build message
        static CAN_message_t msgOut;
        msgOut.ext = 0;
        //change ID format to be better and match my updated plan
        for(auto autoSequence : autoSequenceArray)
        if (autoSequence->getHostNodeID() == nodeID)
        {
            {
                msgOut.id = nodeID + 32;  // with 16 possible nodes in ID format this makes the CAN ID possible go up to 31, lowest sensor ID in current format is 50.
                msgOut.len = 8;
                int64_t autosequenceTimer = autoSequence->getCurrentCountdown();
                uint8_t autosequenceTimerStateEnumToInt = static_cast<uint8_t>(autoSequence->getAutoSequenceState());

                Serial.print("Autosequence: State : ");
                Serial.print(autosequenceTimerStateEnumToInt);
                Serial.print(" Timer : ");
                Serial.print(autosequenceTimer);
                Serial.println();

                msgOut.buf[0] = autosequenceTimerStateEnumToInt;
                msgOut.buf[1] = autosequenceTimer;
                msgOut.buf[2] = (autosequenceTimer >> 8);
                msgOut.buf[3] = (autosequenceTimer >> 16);
                msgOut.buf[4] = (autosequenceTimer >> 24);
                msgOut.buf[5] = (autosequenceTimer >> 32);
                msgOut.buf[6] = (autosequenceTimer >> 40);
                msgOut.buf[7] = (autosequenceTimer >> 48);
                
                // write message to bus
                CANbus.write(msgOut);
            }    
            {
                // add write error handling here, for now it does nothing
            }
            AutoSequenceReportTimer = 0;
        }
        }
}

// Build a CAN report that iterates through and sends all new sensor data
void SensorArrayCANSend(FlexCAN& CANbus, const std::array<MCU_SENSOR*, NUM_SENSORS>& sensorArray)
{
    static CAN_message_t msgOut;
    msgOut.ext = 0;
    //msgOut.len = 2;
    msgOut.len = 5;
    uint32_t sensorValueToSend;
    uint32_t sensorTimeSeconds;
    uint32_t sensorTimeMicros;
    uint8_t sensorTimeSecondsChopped;
    uint16_t sensorTimeMillis;

    for(auto sensor : sensorArray)
    {
        //Serial.println("Assballs");
        //Serial.println(sensor->getNewSensorValueCheck());
        //Serial.print(sensor->getSensorID());
                if (sensor->getNewSensorValueCheck())
        {
            msgOut.id = sensor->getSensorID();
            
            sensorValueToSend = sensor->getCurrentRawValue();
            sensorTimeSeconds = sensor->getTimestampSeconds();
            sensorTimeMicros = sensor->getTimestampMicros();
            sensorTimeSecondsChopped = sensorTimeSeconds % 256;
            sensorTimeMillis = sensorTimeMicros/1000;

            msgOut.buf[0] = sensorValueToSend;
            msgOut.buf[1] = (sensorValueToSend >> 8);
            msgOut.buf[2] = sensorTimeSecondsChopped;
            msgOut.buf[3] = sensorTimeMillis;
            msgOut.buf[4] = (sensorTimeMillis >> 8);

            // write message to bus
            CANbus.write(msgOut);
            //Serial.println("Fuck you you fucking fuck");
            //Serial.println(sensor->getNewSensorValueCheck());
            sensor->setNewSensorValueCheck(false);
            //Serial.println(sensor->getNewSensorValueCheck());
/*             Serial.print("Sensor ID: ");
            Serial.print(msgOut.id);
            Serial.print(" Sensor Value: ");
            Serial.print(sensorValueToSend);
            Serial.print(" Seconds: ");
            Serial.print(sensorTimeSeconds);
            Serial.print(" Micros: ");
            Serial.print(sensorTimeMicros);
            Serial.print(" Millis: ");
            Serial.println(sensorTimeMillis); */
            {
                // add write error handling here, for now it does nothing
            }
        }
    }
}

void SensorArrayConvertedCANSend(FlexCAN& CANbus, const std::array<MCU_SENSOR*, NUM_SENSORS>& sensorArray)
{
    static CAN_message_t msgOut;
    msgOut.ext = 1;                 // Turn ON Extended ID to pack timestamp in the extra bits
    msgOut.len = 2;
    uint32_t sensorValueToSend;
    
    for(auto sensor : sensorArray)
    {
        if (sensor->getNewSensorConversionCheck())
        {
            msgOut.id = sensor->getSensorID() + (sensor->getCANTimestamp())<<11; //packing IDA as normal CAN ID/sensorID and IDB as CAN timestamp
            
            sensorValueToSend = sensor->getCurrentConvertedValue();
            msgOut.buf[0] = sensorValueToSend;
            msgOut.buf[1] = (sensorValueToSend >> 8);

            // write message to bus
            CANbus.write(msgOut);

            // For now don't reset the conversion check, let GUI repeat messages
            //sensor->setNewConversionCheck(false);
            //sensor->setNewSensorValueCheck(false);
/*             Serial.print("Sensor ID");
            Serial.print(msgOut.id);
            Serial.print("Sensor Value");
            Serial.println(sensorValueToSend); */
            {
                // add write error handling here, for now it does nothing
            }
        }
    }
}
/* bool CAN2ValveStateReport()
{
    




} */



////// CAN FD Upgrade Versions //////
// TBD //

/* bool CANFDPropSystemStateReport()
{

} */


/* bool CANFDValveStateReport()
{

} */

#endif