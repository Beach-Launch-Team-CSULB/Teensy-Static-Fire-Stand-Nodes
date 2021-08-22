#include "AbstractedCanPacket.h"
//MiniPacket priority(0,3);
AbstractedCanPacket::AbstractedCanPacket()
{
    ID_Length = 5; //TESTING! Configure for implementation specific address size

    //////////////////////////////////////////Configure priority and nodeID in header file
    priority.setID_Length(priorityID_Length);
    priority.setDataLength(priorityDataLength);


    nodeID.setID_Length(nodeID_ID_Length);
    nodeID.setDataLength(nodeID_DataLength);

    //////////////////////////////////////////Configure overhead bits

    //uint8_t overheadBits = priority.getSize() + nodeID.getSize();
    //maxBufferSize = 29 + (8 * 8) - overheadBits;


    //////////////////////////////////////////initialize our array
    //uint8_t smallestMiniPacketSize = 5; //bits
    //uint8_t maxWeCouldStore = maxBufferSize / smallestMiniPacketSize;
    //MiniPacket storageArray[maxWeCouldStore];
    index = 0;
    //packetBuffer = new MiniPacket[maxWeCouldStore];
    //packetBuffer.setStorage(storageArray, maxWeCouldStore, currentSize);

    //////////////////////////////////////////
    //////////////////////////////////////////
    //////////////////////////////////////////
}

//returns false if priority is too large for MiniPacket priority buffer
bool AbstractedCanPacket::setMessagePriority(uint32_t value)
{
    return priority.setData(value);
}
//returns false if priority is too large for MiniPacket priority buffer
uint32_t AbstractedCanPacket::getMessagePriority()
{
    return priority.getData();
}
//returns false if priority is too large for MiniPacket priority buffer
bool AbstractedCanPacket::setNodeID(uint32_t value)
{
    return nodeID.setID(value);
}
uint32_t AbstractedCanPacket::getNodeID()
{
    return nodeID.getID();
}
uint8_t AbstractedCanPacket::getFreeBits()
{
    return maxBufferSize - usedBits;
}

bool AbstractedCanPacket::canFit(MiniPacket nextPacket)
{
    if (getFreeBits() - nextPacket.getSize()  >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool AbstractedCanPacket::add(MiniPacket nextPacket)
{
    if(canFit(nextPacket))
    {
        packetBuffer[index] = nextPacket;
        usedBits += nextPacket.getSize();
        index++;
        return true;
    }
    return false;//not enough space
}

//TODO TESTING
bool AbstractedCanPacket::send()
{
    return false;
}

MiniPacket* AbstractedCanPacket::getPacketBuffer()
{
    return packetBuffer;
}
uint8_t AbstractedCanPacket::getBufferSize()
{
    return index;
}