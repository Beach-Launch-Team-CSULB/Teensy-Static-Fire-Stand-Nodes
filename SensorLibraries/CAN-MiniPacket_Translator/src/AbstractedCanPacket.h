/*
    An abstracted CAN packet holds the priority of the CAN packet
    it represents, the Node ID, and an arbitrary number of
    MiniPackets.

    It should hold a vector of MiniPackets, as well as keep track
    of how many MiniPackets, it contains, as well as if it can fit
    any more. 
*/

//IMPORTANT CONFIG #DEFINES, THIS IS ALL PEASANTS GET TO TOUCH IN HERE

#define priorityID_Length 0
#define priorityDataLength 3

#define nodeID_ID_Length 5
#define nodeID_DataLength 0

#define smallestMiniPacketSize 5 //size of smallest possible MiniPacket to be sent

//IMPORTANT CONFIG #DEFINES

#include <Arduino.h>
#include <Vector.h>
#include <FlexCAN.h>
#include "MiniPacket.h"

#ifndef AbstractedCanPacker_H
#define AbstractedCanPacker_H


class AbstractedCanPacket
{
public:
    AbstractedCanPacket();


    //for converting MiniPackets to CAN_Frames
    AbstractedCanPacket(uint8_t idLength);
    bool setMessagePriority(uint32_t priority);
    bool setNodeID(uint32_t ID);
    uint8_t getFreeBits();     //returns free space in bits
    bool canFit(MiniPacket nextPacket); //returns true if it can fit the MiniPacket
    bool add(MiniPacket next); //returns true if add successful
    bool send();               //returns true if send successful, false otherwise

    //for converting CAN Frames to MiniPackets
    AbstractedCanPacket(uint8_t idLength, CAN_message_t CAN_Message);
    MiniPacket* getPacketBuffer();
    uint8_t getBufferSize();
    uint32_t getMessagePriority();
    uint32_t getNodeID();

private:
    CAN_message_t msg; //underlying data structure this class abstracts

    uint8_t maxBufferSize; //maximum possible size of abstracted bit buffer
    uint8_t usedBits;      //current size of abstracted bit buffer

    //pre-send overhead
    uint8_t ID_Length;       //in bits. Needs to be known before runtime
    bool extendedID;         //to set when actually building a CAN packet
    uint8_t CAN_Buffer_Size; //for setting msg.len

    //payload data
    // #define priorityID_Length 0
    //#define priorityDataLength 3
    MiniPacket priority; //info contained in data field
    MiniPacket nodeID;   //info contained in ID field

    uint8_t index;

//compiler pre-processing to figure out how big out array needs to be, in order to avoid dynamic allocation
#define overheadBits (priorityID_Length + priorityDataLength + nodeID_ID_Length + nodeID_DataLength)
#define maxBufferSize (29 + (8 * 8) - overheadBits)
#define maxWeCouldStore (maxBufferSize / smallestMiniPacketSize)
    MiniPacket packetBuffer[maxWeCouldStore]; //our array
};

#endif