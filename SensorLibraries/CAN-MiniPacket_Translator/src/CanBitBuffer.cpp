#include "CanBitBuffer.h"
#include <Streaming.h> //testing

uint8_t CanBitBuffer::getMaxBufferSize()
{
    if(msg.ext)
        return 29 + 64;
    else
        return 11 + 64;
}
/*
returns the total number of additional bits which can be written to this CanBitBuffer.
*/
uint8_t CanBitBuffer::getFreeBitsLowLevel()
{
    return getMaxBufferSize() - lowLevelUsedBits;
}
//returns true if it can fit nBits more bits
bool CanBitBuffer::canFitLowLevel(uint8_t nBits)
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

void CanBitBuffer::setExtendedID(bool extendedID)
{
    msg.ext = extendedID;
}
bool CanBitBuffer::getExtendedID()
{
    return msg.ext;
}
//private send helper methods

//returns -1 if msg.id is current buffer, else returns index for msg.buf[index]
int8_t CanBitBuffer::getLowLevelBufferIndex()
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
uint8_t CanBitBuffer::getLowLevelBufferSize()
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
uint8_t CanBitBuffer::getLowLevelBufferFreeSpace()
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
uint8_t CanBitBuffer::getLowLevelBitBoundaryIndex(uint8_t bitWidth)
{
    return getLowLevelBufferFreeSpace() - bitWidth;
}
void CanBitBuffer::setLowLevelBufferBitsHelper(uint32_t data, uint8_t dataWidth, uint8_t dataOffset)
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
    
    if(msg.len>8)//in edge case when bit buffer is totally full, msg.len can erroneously be 9.
        msg.len =8;
}
void CanBitBuffer::setLowLevelBufferBits(uint32_t data, uint8_t dataWidth)
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

//how many bits to pull from CAN bit buffer, offset is how those bits should be shifted before returned
uint32_t CanBitBuffer::readLowLevelBitsHelper(uint8_t bitWidth, uint8_t offset)
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
uint32_t CanBitBuffer::readLowLevelBits(uint8_t bitWidth)
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
CanBitBuffer::CanBitBuffer() 
{
    lowLevelUsedBits=0;
}
//TODO testing
CanBitBuffer::CanBitBuffer(CAN_message_t incomingCAN_Frame) : CanBitBuffer()
{
    msg = incomingCAN_Frame;
}

void CanBitBuffer::reset()
{
    lowLevelUsedBits = 0;
}
//will be deleted, for testing only.
CAN_message_t CanBitBuffer::getCanMessage()
{
    return msg;
}

//useful for testing and debug
void CanBitBuffer::printBits(int data, int size)
{
  for (int i = size - 1; i >= 0; i--)
  {
    Serial.print(bitRead(data, i));
    if (i % 4 == 0)
      Serial.print("");
  }
  Serial.print("-");
}
void CanBitBuffer::printCanMessage()
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