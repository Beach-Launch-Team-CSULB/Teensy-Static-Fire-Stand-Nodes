#include "AbstractedCanPacket.h"
#include <Streaming.h> //testing

void AbstractedCanPacket::init()
{

    //testing Maybe use #define for this??
    //ID_Length = 5; //TESTING! Configure for implementation specific address size

    //Serial.print("in default constructor for AbstractedCANPacket\n");
    //////////////////////////////////////////Configure priority and nodeID in header file
    priority.setID_Length(priorityID_Length);
    priority.setDataLength(priorityDataLength);

    nodeID.setID_Length(nodeID_ID_Length);
    nodeID.setDataLength(nodeID_DataLength);

    packetBufferSize = 0; //is this an unnecessary assignment in C++? YES
    highLevelUsedBits = 0;

}
AbstractedCanPacket::AbstractedCanPacket()
{
    Serial << "Constructor AbstractedCanPacket\n";
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
    //Serial << "nodeID size " << nodeID.getSize();
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
    return msgReplacement.getMaxBufferSize();
}
/*
returns the total number of additional bits which can be written to this AbstractedCanPacket.
High level bits are not for Priority or nodeID, so subtract overheadBits.
*/
uint8_t AbstractedCanPacket::getFreeBitsHighLevel()
{
    return getMaxBufferSize() - overheadBits - highLevelUsedBits;
}
uint8_t AbstractedCanPacket::getFreeBitsLowLevel()
{
    return msgReplacement.getFreeBits();
}
//returns true if it can fit nBits more bits
bool AbstractedCanPacket::canFitHighLevel(uint8_t nBits)
{

    if (getFreeBitsHighLevel() - nBits >= 0)
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
bool AbstractedCanPacket::canFitHighLevel(MiniPacket nextPacket)
{

    //test for a malformed config file
    if (nextPacket.getSize() < smallestMiniPacketSize)
    {
        Serial.print("\n\nERROR, OVERFLOW POSSIBLE, ADJUST smallestMiniPacketSize to ");
        Serial.print(nextPacket.getSize());
        Serial.println(" in AbstractedCanPacket.h");
        return false; //should fail hard rather than soft.
    }
    return canFitHighLevel(nextPacket.getSize());
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
    return msgReplacement.canFit(nextPacket.getSize());
}

/*
adds a MiniPacket to our high-level buffer, and returns false if this fails. 
TODO: update so that the low-level buffer is in sync.
*/
bool AbstractedCanPacket::add(MiniPacket nextPacket)
{
    return highLevelAdd(nextPacket);
    //return lowLevelAdd(nextPacket);
}

//toRead must have ID_Length and dataLength set
MiniPacket AbstractedCanPacket::read(uint8_t ID_Length, uint8_t dataLength)
{
    //important to read ID then Data
    uint32_t ID = msgReplacement.readBits(ID_Length);
    uint32_t data = msgReplacement.readBits(dataLength);
    //Serial << "IN READ: ID: " << ID << "Data: " << data << endl;
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
    return 5;
}
MiniPacket AbstractedCanPacket::read(uint8_t ID_Length)
{
    MiniPacket toReturn;
    toReturn.setID_Length(ID_Length);
    toReturn.setID(msgReplacement.readBits(ID_Length));

    if (toReturn.getID() == 0) //null id i.e. nothing here
        return toReturn;       //make sure to test before adding a null MiniPacket

    uint8_t dataLength = packetIdToDataLength(getNodeID(), toReturn.getID());
    toReturn.setDataLength(dataLength);
    toReturn.setData(msgReplacement.readBits(dataLength));

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
    msgReplacement.setExtendedID(extendedID);
}
bool AbstractedCanPacket::getExtendedID()
{
    return msgReplacement.getExtendedID();
}
//private send helper methods

/*
returns the size of the low-level data unit we're writing to. 
This can be 29, 11, or 8.
*/
uint8_t AbstractedCanPacket::getLowLevelBufferSize()
{
    uint8_t idSize;
    if (getExtendedID() == true) //idSize is 29 bits
        idSize = 29;
    else //idSize is 11 bits
        idSize = 11;

    if (lowLevelUsedBits < idSize)
        return idSize;
    else
        return 8;
}

/*
    public add function will update high level MiniPacket buffer, this function
    will update the private CAN_message_t within it. Also, this method updates
    usedBits.
    */
bool AbstractedCanPacket::lowLevelAdd(MiniPacket nextPacket, bool errorChecking)
{
    if (canFitLowLevel(nextPacket, errorChecking))
    {
        msgReplacement.writeBits(nextPacket.getID(), nextPacket.getID_Length());
        msgReplacement.writeBits(nextPacket.getData(), nextPacket.getDataLength());
        return true;
    }

    Serial.print("ERROR\n\nAborted Packet: ");
    nextPacket.print();
    Serial << "Free bits low level: " << msgReplacement.getFreeBits() << endl;

    Serial.println();
    return false;
}

bool AbstractedCanPacket::highLevelAdd(MiniPacket nextPacket)
{

    //make sure there's room
    if (canFitHighLevel(nextPacket))
    {
        packetBuffer[packetBufferSize] = nextPacket;
        highLevelUsedBits += nextPacket.getSize();
        packetBufferSize++;
        return true;
    }
    return false; //not enough space
}

//TODO testing
AbstractedCanPacket::AbstractedCanPacket(uint8_t idLength, CAN_message_t incomingCAN_Frame) : msgReplacement(incomingCAN_Frame)
{
    init();
    Serial << "msgReplacement CAN Frame:\n";
    msgReplacement.printCanMessage();
    Serial << endl;    
    Serial << "msgReplacement extID: " << msgReplacement.getExtendedID() << endl;

    //Serial << "low level read in Constructor: \n";
    //Serial << "lowLevelUsedBits: " << lowLevelUsedBits;

    //printCanMessage();
    //Serial << "\nreceived CAN Frame ";
    //Serial << endl;
    priority = read(priorityID_Length, priorityDataLength);
    nodeID = read(nodeID_ID_Length, nodeID_DataLength);
    /*
    Serial.print("\nhere\nPriority: ");
    priority.print();
    Serial.print("NodeID: ");
    nodeID.print();
    Serial.println();
    //*/
    //Serial <<"canFitLowLevel :" << canFitLowLevel(idLength) << endl;
    int counter = 0;
    while (msgReplacement.canFit(idLength))
    {
        MiniPacket next = read(idLength);
        /*
        next.print();
        Serial.println();
        //*/
        if (next.getID() != 0) //if next_ID != nullID
            highLevelAdd(next);
        
        
        //Serial << "maxBufferSize: " << maxBufferSize << endl;
        //Serial << "usedBitsLowLevel: " << lowLevelUsedBits << endl;
        //Serial << "getFreeBitsLowLevel: " << getFreeBitsLowLevel() << endl << endl;
    }
}

void AbstractedCanPacket::reset()
{
    //lowLevelUsedBits = 0;
    highLevelUsedBits = 0;
    packetBufferSize = 0;
}
void AbstractedCanPacket::writeToCAN()
{
    //Serial << "START WRITE TO CAN\n\n";
    //lowLevelUsedBits = 0; //reset so we can rewrite data.
    msgReplacement.reset();
    lowLevelAdd(priority, false); //false means no error checking
    //Serial << "added Priority" << endl;
    //Serial << "Priority size: " << priority.getSize() << endl;
    //Serial << "Free bits low level: " << getFreeBitsLowLevel() << endl
    //       << endl;

    lowLevelAdd(nodeID, false);
    //Serial << "added nodeID" << endl;
    //Serial << "nodeID size: " << nodeID.getSize() << endl;
    //Serial << "Free bits low level: " << getFreeBitsLowLevel() << endl
    //       << endl;

    for (int i = 0; i < packetBufferSize; i++)
    {
        lowLevelAdd(packetBuffer[i]);

        //Serial << "added packetBuffer[" << i << "]" << endl;
        //Serial << "packetBuffer[" << i << "] size: " << packetBuffer[i].getSize() << endl;
        //Serial << "Free bits low level: " << getFreeBitsLowLevel() << endl
        //       << endl;
    }
}

//will be deleted, for testing only.
CAN_message_t AbstractedCanPacket::getCanMessage()
{
    return msgReplacement.getCanMessage();
}

//useful for testing and debug
void printBits(int data, int size)
{
    for (int i = size - 1; i >= 0; i--)
    {
        Serial.print(bitRead(data, i));
        if (i % 4 == 0)
            Serial.print("");
    }
    Serial.print("-");
}
void AbstractedCanPacket::printCanMessage()
{
    msgReplacement.printCanMessage();
}
