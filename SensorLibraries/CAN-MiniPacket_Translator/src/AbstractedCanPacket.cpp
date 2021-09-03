#include "AbstractedCanPacket.h"
#include <Streaming.h> 

void AbstractedCanPacket::init()
{
    //Configure priority and nodeID in header file
    priority.setID_Length(priorityID_Length);
    priority.setDataLength(priorityDataLength);

    nodeID.setID_Length(nodeID_ID_Length);
    nodeID.setDataLength(nodeID_DataLength);

    packetBufferSize = 0; 
    usedBits = 0;

}
AbstractedCanPacket::AbstractedCanPacket()
{
    setExtendedID(true); //default to extendedID format
    init();
}

//returns false if newPriority is too large for MiniPacket priority buffer, otherwise sets priority
bool AbstractedCanPacket::setMessagePriority(uint32_t newPriority)
{
    return priority.setData(newPriority);
}
//returns the priority value of this AbstractedCanPacket
uint32_t AbstractedCanPacket::getMessagePriority()
{
    return priority.getData();
}

//returns false if newID is too large for MiniPacket ID buffer
bool AbstractedCanPacket::setNodeID(uint32_t newID)
{
    return nodeID.setID(newID);
}

/*
returns the ID of this AbstractedCanPacket.
*/
uint32_t AbstractedCanPacket::getNodeID()
{
    return nodeID.getID();
}

uint8_t AbstractedCanPacket::getMaxBufferSize()
{
    return msg.getMaxBufferSize();
}
/*
returns the total number of additional bits which can be written to this AbstractedCanPacket.
High level bits are not for Priority or nodeID, so subtract overheadBits.
*/
uint8_t AbstractedCanPacket::getFreeBits()
{
    return getMaxBufferSize() - overheadBits - usedBits;
}
//returns true if it can fit nBits more bits
bool AbstractedCanPacket::canFit(uint8_t nBits)
{

    if (getFreeBits() - nBits >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
returns true if this AbstractedCanPacket can fit nextPacket, false otherwise. 
*/
bool AbstractedCanPacket::canFit(MiniPacket nextPacket)
{

    //test for a malformed config file
    if (nextPacket.getSize() < smallestMiniPacketSize)
    {
        Serial.print("\n\nERROR, OVERFLOW POSSIBLE, ADJUST smallestMiniPacketSize to ");
        Serial.print(nextPacket.getSize());
        Serial.println(" in AbstractedCanPacket.h");
        return false; //should fail hard rather than soft.
    }
    return canFit(nextPacket.getSize());
}
/*
returns true if this AbstractedCanPacket can fit nextPacket, false otherwise. 
*/
bool AbstractedCanPacket::canFitLowLevel(MiniPacket nextPacket, bool errorChecking)
{

    //test for a malformed config file
    //should only skip error checking if adding a priority or nodeID MiniPacket
    if (errorChecking)
    {
        if (nextPacket.getSize() < smallestMiniPacketSize)
        {
            Serial.print("\n\nERROR, OVERFLOW POSSIBLE, ADJUST smallestMiniPacketSize to ");
            Serial.print(nextPacket.getSize());
            Serial.println(" in AbstractedCanPacket.h");
            return false; //should fail hard rather than soft.
        }
    }
    return msg.canFit(nextPacket.getSize());
}


//toRead must have ID_Length and dataLength set
MiniPacket AbstractedCanPacket::read(uint8_t ID_Length, uint8_t dataLength)
{
    //important to read ID then Data
    uint32_t ID = msg.readBits(ID_Length);
    uint32_t data = msg.readBits(dataLength);
    MiniPacket toReturn;

    toReturn.setDataLength(dataLength);
    toReturn.setID_Length(ID_Length);
    toReturn.setID(ID);
    toReturn.setData(data);

    return toReturn;
}

/*
    TODO Testing
    This is the function mapping of our packetID to the length of data.
    Each sensor in each node should have a unique ID, and code will go here to 
    map that ID to how long its data is.

    This is incomplete, but I'm architecting so it's easy to improve later. 

    I'm considering implementing a 2D array for this mapping, where
    the first dimension is the nodeID, and the second dimension is the 
    MiniPacket ID unique to that node, where MiniPacket ID corresponds
    to what sensor or other type of data is trying to be sent. 
*/
uint8_t AbstractedCanPacket::packetIdToDataLength(uint8_t nodeID, uint8_t packetID)
{
    if (nodeID == 0) //zero is null ID
    {
        return 0;
    }
    return 12;
}
MiniPacket AbstractedCanPacket::read(uint8_t ID_Length)
{
    MiniPacket toReturn;
    toReturn.setID_Length(ID_Length);
    toReturn.setID(msg.readBits(ID_Length));

    if (toReturn.getID() == 0) //null id i.e. nothing here
        return toReturn;       //make sure to test before adding a null MiniPacket

    uint8_t dataLength = packetIdToDataLength(getNodeID(), toReturn.getID());
    toReturn.setDataLength(dataLength);
    toReturn.setData(msg.readBits(dataLength));

    return toReturn;
}

/*
returns an array containing our MiniPackets.
*/
MiniPacket *AbstractedCanPacket::getPacketBuffer()
{
    return packetBuffer;
}
/*
returns the number of MiniPackets in this AbstractedCanPacket.
*/
uint8_t AbstractedCanPacket::getBufferSize()
{
    return packetBufferSize;
}

void AbstractedCanPacket::setExtendedID(bool extendedID)
{
    msg.setExtendedID(extendedID);
}
bool AbstractedCanPacket::getExtendedID()
{
    return msg.getExtendedID();
}
//private send helper methods

//This method adds a MiniPacket to
bool AbstractedCanPacket::addLowLevel(MiniPacket nextPacket, bool errorChecking)
{
    if (canFitLowLevel(nextPacket, errorChecking))
    {
        msg.writeBits(nextPacket.getID(), nextPacket.getID_Length());
        msg.writeBits(nextPacket.getData(), nextPacket.getDataLength());
        return true;
    } 
    return false;

}

bool AbstractedCanPacket::add(MiniPacket nextPacket)
{

    //make sure there's room
    if (canFit(nextPacket))
    {
        packetBuffer[packetBufferSize] = nextPacket;
        usedBits += nextPacket.getSize();
        packetBufferSize++;
        return true;
    }
    return false; //not enough space
}

AbstractedCanPacket::AbstractedCanPacket(uint8_t idLength, CAN_message_t incomingCAN_Frame) : msg(incomingCAN_Frame)
{
    init();

    priority = read(priorityID_Length, priorityDataLength);
    nodeID = read(nodeID_ID_Length, nodeID_DataLength);

    while (msg.canFit(idLength))
    {
        MiniPacket next = read(idLength);

        if (next.getID() != 0) //if next_ID != nullID
            add(next);
    }
}

void AbstractedCanPacket::reset()
{
    usedBits = 0;
    packetBufferSize = 0;
}
void AbstractedCanPacket::writeToCAN()
{
    msg.reset();
    addLowLevel(priority, false); //false means no error checking


    addLowLevel(nodeID, false);

    for (int i = 0; i < packetBufferSize; i++)
    {
        addLowLevel(packetBuffer[i]);
    }
}

CAN_message_t AbstractedCanPacket::getCanMessage()
{
    writeToCAN();//make sure to write to the low-level buffer before returning it.
    return msg.getCanMessage();
}

void AbstractedCanPacket::printCanMessage()
{
    msg.printCanMessage();
}
