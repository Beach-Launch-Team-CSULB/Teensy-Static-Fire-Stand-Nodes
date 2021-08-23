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

    //////////////////////////////////////////initialize our array
    index = 0;

    setExtendedID(true);//default to extendedID format
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

bool AbstractedCanPacket::add(MiniPacket nextPacket)
{

    //make sure there's room
    if (canFit(nextPacket))
    {
        packetBuffer[index] = nextPacket;
        usedBits += nextPacket.getSize();
        index++;
        return true;
    }
    return false; //not enough space
}

MiniPacket *AbstractedCanPacket::getPacketBuffer()
{
    return packetBuffer;
}
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

    return 8 - (tempUsedBits % 8); //remainder after dividing by 8
}
uint8_t AbstractedCanPacket::getLowLevelBitBoundaryIndex(uint8_t bitWidth)
{
    return getLowLevelBufferFreeSpace() - bitWidth;
}
void AbstractedCanPacket::setLowLevelBufferBitsHelper(uint32_t data, uint8_t dataWidth, uint8_t dataOffset)
{
    config compression = {dataWidth, dataOffset};
    
    uint8_t destinationOffset = getLowLevelBitBoundaryIndex(dataWidth) ;
    config extraction = {dataWidth, destinationOffset};

    BitChopper bitChopper;

    uint32_t selectedData = bitChopper.compress(compression, data);
    uint32_t dataToWrite = bitChopper.extract(extraction, selectedData);

    int8_t index = getLowLevelBufferIndex();
    Serial.print("index: "); Serial.println(index);
    if(index == -1)
        msg.id = msg.id | dataToWrite;
    else
        msg.buf[index] = msg.buf[index] | (uint8_t) dataToWrite;

    usedBits += dataWidth;
}
void AbstractedCanPacket::setLowLevelBufferBits(uint32_t data, uint8_t dataWidth)
{
    while(dataWidth > 0)
    {

        uint8_t dataWidthHelper = min(getLowLevelBufferFreeSpace(), dataWidth);
        uint8_t dataOffset = dataWidth - dataWidthHelper;
        setLowLevelBufferBitsHelper(data, dataWidthHelper, dataOffset);
        dataWidth -= dataWidthHelper;
    }
}

CAN_message_t AbstractedCanPacket::getCanMessage()
{
    return msg;
}
//TODO TESTING
bool AbstractedCanPacket::send()
{
    return false;
}