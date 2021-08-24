#include "AbstractedCanPacket.h"
AbstractedCanPacket::AbstractedCanPacket()
{
    //testing Maybe use #define for this??
    //ID_Length = 5; //TESTING! Configure for implementation specific address size

    //////////////////////////////////////////Configure priority and nodeID in header file
    priority.setID_Length(priorityID_Length);
    priority.setDataLength(priorityDataLength);

    nodeID.setID_Length(nodeID_ID_Length);
    nodeID.setDataLength(nodeID_DataLength);

    index = 0; //is this an unnecessary assignment in C++?

    setExtendedID(true); //default to extendedID format
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

/*
returns the total number of additional bits which can be written to this AbstractedCanPacket.
*/
uint8_t AbstractedCanPacket::getFreeBits()
{
    return maxBufferSize - usedBits;
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

    if (getFreeBits() - nextPacket.getSize() >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
adds a MiniPacket to our high-level buffer, and returns false if this fails. 
TODO: update so that the low-level buffer is in sync.
*/
bool AbstractedCanPacket::add(MiniPacket nextPacket)
{

    //make sure there's room
    if (canFit(nextPacket))
    {
        packetBuffer[index] = nextPacket;
        lowLevelAdd(nextPacket);
        index++;
        return true;
    }
    return false; //not enough space
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
    return index;
}

void AbstractedCanPacket::setExtendedID(bool extendedID)
{
    msg.ext = extendedID;
}
bool AbstractedCanPacket::getExtendedID()
{
    return msg.ext;
}
//private send helper methods

//returns -1 if msg.id is current buffer, else returns index for msg.buf[index]
int8_t AbstractedCanPacket::getLowLevelBufferIndex()
{
    uint8_t tempUsedBits = usedBits;
    uint8_t idSize;
    if (getExtendedID() == true) //idSize is 29 bits
        idSize = 29;
    else //idSize is 11 bits
        idSize = 11;

    if (tempUsedBits < idSize)
        return -1;
    else
        tempUsedBits -= idSize;

    return tempUsedBits / 8;
}
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

    if (usedBits < idSize)
        return idSize;
    else
        return 8;
}
/*
returns the index of leftmost free bit, 
which is also the size of the free space of the current 
buffer we are writing to (id or buf[i])
*/
uint8_t AbstractedCanPacket::getLowLevelBufferFreeSpace()
{

    uint8_t tempUsedBits = usedBits;
    uint8_t idSize;
    if (getExtendedID() == true) //idSize is 29 bits
        idSize = 29;
    else //idSize is 11 bits
        idSize = 11;

    if (tempUsedBits < idSize)
        return idSize - tempUsedBits;
    else
        tempUsedBits -= idSize;

    return 8 - (tempUsedBits % 8); //usedBits % 8 is used space, 8 - that is free space
}
/*
Returns the leftmost index which can bit data of bitWidth size without overwriting data on the left. 
*/
uint8_t AbstractedCanPacket::getLowLevelBitBoundaryIndex(uint8_t bitWidth)
{
    return getLowLevelBufferFreeSpace() - bitWidth;
}
void AbstractedCanPacket::setLowLevelBufferBitsHelper(uint32_t data, uint8_t dataWidth, uint8_t dataOffset)
{
    config compression = {dataWidth, dataOffset}; //take relevant bits our of data

    uint8_t destinationOffset = getLowLevelBitBoundaryIndex(dataWidth); //find leftmost index which will contain data (pack from left to right)
    config extraction = {dataWidth, destinationOffset};                 //mapping for where our relevant bits go

    BitChopper bitChopper;

    //maps relevant bits in input to the right offset for the output
    uint32_t selectedData = bitChopper.compress(compression, data);
    uint32_t dataToWrite = bitChopper.extract(extraction, selectedData);

    int8_t index = getLowLevelBufferIndex(); //figure out which part of msg to write to
    if (index == -1)                         //write to id
        msg.id = msg.id | dataToWrite;
    else //write to buf
    {
        msg.buf[index] = msg.buf[index] | (uint8_t)dataToWrite;
        msg.len = index + 1; //update length to make sure the message sends properly
    }

    usedBits += dataWidth; //update to indicate we wrote to CAN buffer
}
void AbstractedCanPacket::setLowLevelBufferBits(uint32_t data, uint8_t dataWidth)
{
    while (dataWidth > 0)
    {
        //you can only write as many bits the smaller of the current buffer free space and our input dataWidth
        uint8_t dataWidthHelper = min(getLowLevelBufferFreeSpace(), dataWidth);
        //start writing from the MSB to LSB, ie write left side first. Simplifies to 0 if we can write dataWidth bits
        uint8_t dataOffset = dataWidth - dataWidthHelper;
        setLowLevelBufferBitsHelper(data, dataWidthHelper, dataOffset);
        dataWidth -= dataWidthHelper;
    }
}

/*
    public add function will update high level MiniPacket buffer, this function
    will update the private CAN_message_t within it. Also, this method updates
    usedBits.
    */
void AbstractedCanPacket::lowLevelAdd(MiniPacket nextPacket)
{
    setLowLevelBufferBits(nextPacket.getID(), nextPacket.getID_Length());
    setLowLevelBufferBits(nextPacket.getData(), nextPacket.getDataLength());
}

//how many bits to pull from CAN bit buffer, offset is how those bits should be shifted before returned
uint32_t AbstractedCanPacket::readLowLevelBitsHelper(uint8_t bitWidth, uint8_t offset)
{
    int8_t bufferIndex = getLowLevelBufferIndex();
    //Serial.print("bufferIndex: ");
    //Serial.println(bufferIndex);
    uint8_t bitIndex = getLowLevelBitBoundaryIndex(bitWidth);
    config compression = {bitWidth, bitIndex};
    config extraction = {bitWidth, offset};


    BitChopper bitChopper;

    uint32_t data;
    if(bufferIndex<0)
    {
        data = msg.id;
    }
    else
    {
        data = msg.buf[bufferIndex];
    }

    //maps relevant bits in input to the right offset for the output
    uint32_t selectedData = bitChopper.compress(compression, data);
    uint32_t dataToWrite = bitChopper.extract(extraction, selectedData);
    
    usedBits += bitWidth;
    return dataToWrite;
}
uint32_t AbstractedCanPacket::readLowLevelBits(uint8_t bitWidth)
{
    uint32_t toReturn =0;
    while (bitWidth > 0)
    {
        //you can only write as many bits the smaller of the current buffer free space and our input dataWidth
        uint8_t dataWidthHelper = min(getLowLevelBufferFreeSpace(), bitWidth);
        //start writing from the MSB to LSB, ie write left side first. Simplifies to 0 if we can write dataWidth bits
        uint8_t dataOffset = bitWidth - dataWidthHelper;
        toReturn = toReturn | readLowLevelBitsHelper(dataWidthHelper, dataOffset);
        bitWidth -= dataWidthHelper;
    }
    return toReturn;
}


//TODO testing
AbstractedCanPacket::AbstractedCanPacket(uint8_t idLength, CAN_message_t CAN_Message)
{
    AbstractedCanPacket();
}

void AbstractedCanPacket::reset()
{
    usedBits =0;
    index = 0;
}
//will be deleted, for testing only.
CAN_message_t AbstractedCanPacket::getCanMessage()
{
    return msg;
}
//Can_send proxy TODO testing
bool AbstractedCanPacket::send()
{
    return false;
}