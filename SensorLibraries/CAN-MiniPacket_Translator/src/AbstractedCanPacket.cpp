#include "AbstractedCanPacket.h"
#include <Streaming.h> //testing


AbstractedCanPacket::AbstractedCanPacket()
{
    //testing Maybe use #define for this??
    //ID_Length = 5; //TESTING! Configure for implementation specific address size

    //Serial.print("in default constructor for AbstractedCANPacket\n");
    //////////////////////////////////////////Configure priority and nodeID in header file
    priority.setID_Length(priorityID_Length);
    priority.setDataLength(priorityDataLength);

    nodeID.setID_Length(nodeID_ID_Length);
    nodeID.setDataLength(nodeID_DataLength);

    packetBufferSize = 0; //is this an unnecessary assignment in C++?
    lowLevelUsedBits =0;
    highLevelUsedBits = 0;


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

/*
returns the total number of additional bits which can be written to this AbstractedCanPacket.
High level bits are not for Priority or nodeID, so subtract overheadBits.
*/
uint8_t AbstractedCanPacket::getFreeBitsHighLevel()
{
    return maxBufferSize - overheadBits - highLevelUsedBits;
}
/*
returns the total number of additional bits which can be written to this AbstractedCanPacket.
*/
uint8_t AbstractedCanPacket::getFreeBitsLowLevel()
{
    return maxBufferSize - lowLevelUsedBits;
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
//returns true if it can fit nBits more bits
bool AbstractedCanPacket::canFitLowLevel(uint8_t nBits)
{

    if (getFreeBitsLowLevel() - nBits >= 0)
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
    return canFitLowLevel(nextPacket.getSize());
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
    uint32_t ID = readLowLevelBits(ID_Length);
    uint32_t data = readLowLevelBits(dataLength);
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
    toReturn.setID(readLowLevelBits(ID_Length));

    if(toReturn.getID() == 0)//null id ie nothing here
        return toReturn;//make sure to test before adding a null MiniPacket

    uint8_t dataLength = packetIdToDataLength(getNodeID(), toReturn.getID());
    toReturn.setDataLength(dataLength);
    toReturn.setData(readLowLevelBits(dataLength));

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
    uint8_t tempUsedBits = lowLevelUsedBits;
    //Serial << "in getLowLevelBufferIndex: \n";
    //Serial << "lowLevelUsedBits: " << lowLevelUsedBits << endl;
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

    if (lowLevelUsedBits < idSize)
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

    uint8_t tempUsedBits = lowLevelUsedBits;
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
Returns the leftmost index which data of bitWidth size can be written to without overwriting data on the left. 
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
    }

    lowLevelUsedBits += dataWidth; //update to indicate we wrote to CAN buffer

    //must be done after updating usedBits to ensure index is consistent
    msg.len = getLowLevelBufferIndex() + 1; //update length to make sure the message sends properly
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
bool AbstractedCanPacket::lowLevelAdd(MiniPacket nextPacket, bool errorChecking)
{
    if (canFitLowLevel(nextPacket, errorChecking))
    {
        setLowLevelBufferBits(nextPacket.getID(), nextPacket.getID_Length());
        setLowLevelBufferBits(nextPacket.getData(), nextPacket.getDataLength());
        return true;
    }

    Serial.print("ERROR\n\nAborted Packet: ");
    nextPacket.print();
    Serial << "Free bits low level: " << getFreeBitsLowLevel() << endl;

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

//how many bits to pull from CAN bit buffer, offset is how those bits should be shifted before returned
uint32_t AbstractedCanPacket::readLowLevelBitsHelper(uint8_t bitWidth, uint8_t offset)
{
    int8_t bufferIndex = getLowLevelBufferIndex();
    //Serial << "readLowLevelBitsHelper: \n";
    //Serial.print("bufferIndex: ");
    //Serial.println(bufferIndex);
    uint8_t bitIndex = getLowLevelBitBoundaryIndex(bitWidth);
    config compression = {bitWidth, bitIndex};
    config extraction = {bitWidth, offset};

    BitChopper bitChopper;

    uint32_t data;
    if (bufferIndex < 0)
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

    lowLevelUsedBits += bitWidth;
    return dataToWrite;
}
uint32_t AbstractedCanPacket::readLowLevelBits(uint8_t bitWidth)
{
    uint32_t toReturn = 0;
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
AbstractedCanPacket::AbstractedCanPacket(uint8_t idLength, CAN_message_t incomingCAN_Frame) : AbstractedCanPacket()
{
    msg = incomingCAN_Frame;
    //Serial << "low level read in Constructor: \n";
    //Serial << "lowLevelUsedBits: " << lowLevelUsedBits;

    
    //printCanMessage(); 
    //Serial << "\nreceived CAN Frame ";
    //Serial << endl;
    priority = read(priorityID_Length, priorityDataLength);
    nodeID = read(nodeID_ID_Length, nodeID_DataLength);
    Serial.print("\nhere\nPriority: ");
    priority.print();
    Serial.print("NodeID: ");
    nodeID.print();
    Serial.println();

    Serial <<"canFitLowLevel :" << canFitLowLevel(idLength) << endl;
    int counter = 0;
    while (canFitLowLevel(idLength))
    {
        MiniPacket next = read(idLength);
        next.print();
        Serial.println();
        if (next.getID() != 0)//if next_ID != nullID
            highLevelAdd(next);
        counter++;
        while (counter > 11)
        {
            //Serial << "here";
            delay(1000);
        }
        Serial << "maxBufferSize: " << maxBufferSize << endl;
        Serial << "usedBitsLowLevel: " << lowLevelUsedBits << endl;
        Serial << "getFreeBitsLowLevel: " << getFreeBitsLowLevel() << endl << endl;
    }
}

void AbstractedCanPacket::reset()
{
    lowLevelUsedBits = 0;
    highLevelUsedBits = 0;
    packetBufferSize = 0;
}
void AbstractedCanPacket::writeToCAN()
{
    //Serial << "START WRITE TO CAN\n\n";
    lowLevelUsedBits = 0; //reset so we can rewrite data.

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
    return msg;
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
  uint8_t idSize;
  if (msg.ext)
  {
    idSize = 29;
  }
  else
  {
    idSize = 11;
  }
  printBits(msg.id, idSize);
  for (int i = 0; i < msg.len; i++)
  {
    printBits(msg.buf[i], 8);
  }
}
//Can_send proxy TODO testing
bool AbstractedCanPacket::send()
{
    return false;
}