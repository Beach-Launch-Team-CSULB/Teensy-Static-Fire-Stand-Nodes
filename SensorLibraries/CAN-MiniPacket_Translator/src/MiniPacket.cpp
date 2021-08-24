#include "MiniPacket.h"

MiniPacket::MiniPacket()
{
    ID_Length = 5; //TESTING Implementation specific config
    dataLength = 7;
}
MiniPacket::MiniPacket(uint8_t ID_Length, uint8_t dataLength)
{
    MiniPacket::ID_Length = ID_Length;
    MiniPacket::dataLength = dataLength;
}
uint8_t MiniPacket::getSize()
{
    return ID_Length + dataLength;
}

void MiniPacket::setID_Length(uint32_t ID_Length)
{
    MiniPacket::ID_Length = ID_Length;
}
uint8_t MiniPacket::getID_Length()
{
    return ID_Length;
}

void MiniPacket::setDataLength(uint32_t dataLength)
{
    MiniPacket::dataLength = dataLength;
}
uint8_t MiniPacket::getDataLength()
{
    return dataLength;
}
bool MiniPacket::setData(uint32_t data)
{

    //error checking
    uint32_t mask = (1 << dataLength) - 1;
    if ((data & mask) == data) //make sure no information is lost in encoding
    {
        MiniPacket::data = data;
        return true;
    }
    else
        return false;
}
uint32_t MiniPacket::getData()
{
    return data;
}

//returns true if ID field set successfully, false if ID is too big for this MiniPacket
bool MiniPacket::setID(uint32_t ID)
{

    //error checking
    uint32_t mask = (1 << ID_Length) - 1;
    if ((ID & mask) == ID) //make sure no information is lost
    {
        MiniPacket::ID = ID;
        return true;
    }
    else
        return false;
}
uint32_t MiniPacket::getID()
{
    return ID;
}

//useful for testing and debug
void MiniPacket::printBits(int data, int size)
{
  for(int i = size-1; i >= 0; i--)
  {
    Serial.print(bitRead(data,i));
    if(i % 4 == 0) Serial.print("");
  }  
  Serial.print("");
}
void MiniPacket::print()
{
    Serial.print("ID: ");
    printBits(ID, ID_Length);
    Serial.print(", DATA: ");
    printBits(data, dataLength);
}