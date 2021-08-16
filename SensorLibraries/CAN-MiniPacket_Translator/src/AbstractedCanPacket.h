/*
    An abstracted CAN packet holds the priority of the CAN packet
    it represents, the Node ID, and an arbitrary number of
    MiniPackets.

    It should hold a vector of MiniPackets, as well as keep track
    of how many MiniPackets, it contains, as well as if it can fit
    any more. 
*/

#include <Arduino.h>
#include <Vector.h>
#include <FlexCAN.h>

#ifndef AbstractedCanPacker_H
#define AbstractedCanPacker_H
typedef struct MiniPacket
{
    uint32_t ID;
    uint8_t ID_Length; //not sent

    uint32_t data;
    uint8_t dataLength; //not sent
} MiniPacket;

class AbstractedCanPacket
{
public:
    AbstractedCanPacket();

    //for converting MiniPackets to CAN_Frames
    AbstractedCanPacket(uint8_t idLength);
    bool add(MiniPacket next);          //returns true if add successful
    bool canFit(MiniPacket nextPacket); //returns true if it can fit the MiniPacket
    bool send();                        //returns true if send successful, false otherwise

    //for converting CAN Frames to MiniPackets
    AbstractedCanPacket(uint8_t idLength, CAN_message_t CAN_Message);
    MiniPacket getPriority();
    MiniPacket getNodeID();
    Vector<MiniPacket> getBuffer();

private:
    CAN_message_t msg; //underlying data structure this class abstracts

    uint8_t maxBufferSize; //maximum possible size of abstracted bit buffer
    uint8_t usedBits;      //current size of abstracted bit buffer

    //pre-send overhead
    uint8_t ID_Length;       //in bits. Needs to be known before runtime
    bool extendedID;         //to set when actually building a CAN packet
    uint8_t CAN_Buffer_Size; //for setting msg.len

    //payload data
    MiniPacket priority;
    MiniPacket nodeID;
    Vector<MiniPacket> packetBuffer;
};

#endif