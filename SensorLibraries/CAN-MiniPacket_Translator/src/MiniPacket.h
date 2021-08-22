#ifndef MiniPacket_h
#define MiniPacket_h
#include <Arduino.h>

class MiniPacket
{
public:
    MiniPacket();
    MiniPacket(uint8_t IdLength, uint8_t dataLength);
    uint8_t getSize();
    void setID_Length(uint32_t ID_Length);
    void setDataLength(uint32_t dataLength);
    bool setData(uint32_t data);
    uint32_t getData();
    bool setID(uint32_t ID);
    uint32_t getID();
    void print();
private:
    uint32_t ID;
    uint8_t ID_Length; //size in bits of ID buffer, not transfered over CAN

    uint32_t data;
    uint8_t dataLength; //size in bits of data buffer, not transfered over CAN
};

#endif