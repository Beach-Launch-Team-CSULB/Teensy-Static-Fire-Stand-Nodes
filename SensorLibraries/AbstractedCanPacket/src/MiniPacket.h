/*
    Author Jacob Waters
    fierywaters13@gmail.com
    Designed for CSULB Beach Launch Team
*/
#ifndef MiniPacket_h
#define MiniPacket_h
#include <Arduino.h>

class MiniPacket
{
public:
    MiniPacket();
    MiniPacket(uint8_t IdLength, uint8_t dataLength);
    uint8_t getSize();//total size of MiniPacket

    void setID_Length(uint8_t ID_Length);
    uint8_t getID_Length();

    void setDataLength(uint8_t dataLength);
    uint8_t getDataLength();

    bool setData(uint32_t data);
    uint32_t getData();

    //sets the ID of the MiniPacket, cannot be zero, and must fit into ID_Size bits
    bool setID(uint32_t ID);
    uint32_t getID();

    uint32_t truncate(uint32_t data, uint8_t bitWidth);

    void printBits(int data, int size);//helper
    void print();
private:
    uint32_t ID;
    uint8_t ID_Length; //size in bits of ID buffer, not transfered over CAN

    uint32_t data;
    uint8_t dataLength; //size in bits of data buffer, not transfered over CAN
};

#endif