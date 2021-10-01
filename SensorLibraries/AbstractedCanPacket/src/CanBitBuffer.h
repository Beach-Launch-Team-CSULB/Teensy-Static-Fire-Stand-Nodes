/*
    Author Jacob Waters
    fierywaters13@gmail.com
    Designed for CSULB Beach Launch Team
*/
/*
    CanBitBuffer is is a tool that represents a CAN Packet as a bit buffer which
    can be written to as if it were continuous, rather than an ID field and
    up to 8 separate bytes
*/
// 29 + 8*8 bit 
#include <Arduino.h>
#include <FlexCAN.h>
#include "BitChopper.h"

#ifndef CanBitBuffer_H
#define CanBitBuffer_H

class CanBitBuffer
{
public:
    void init();
    CanBitBuffer();
    CanBitBuffer(CAN_message_t msg);

    void writeBits(uint32_t data, uint8_t dataWidth);
    uint32_t readBits(uint8_t bitWidth);

    uint8_t getFreeBits();      //returns free space in bits
    bool canFit(uint8_t nBits); //returns true if it can fit nBits more bits. Should be private

    CAN_message_t getCanMessage(); 

    //CAN frame config
    void setExtendedID(bool extID);
    bool getExtendedID();

    void reset(); //resets can packet to be reused. However, old data is not deleted.

    //private:
    CAN_message_t msg; //underlying data structure this class abstracts

    //uint8_t maxBufferSize; //maximum possible size of abstracted bit buffer TESTING

    /*
    usedBits is essentially the high-level index of where we are in the abstract bit buffer
    Ie how many bits we've used out of the total number of bits we have considered continuously.

    When converting MiniPackets to CAN frames, usedBits corresponds with the total number of
    bits we've written to the CAN_message_t. However, when AbstractedCanPacket is constructed
    from a CAN frame, converting to MiniPackets, usedBits refers to how many bits we have read
    from the CAN frame.

    Note, that after the constructor finishes, usedBits will simply be the sum of the sizes
    of all the MiniPackets this AbstractedCanPacket contains.
    */

    uint8_t usedBits; //current size of the low level bit buffer

    //compiler pre-processing to figure out how big out array needs to be, in order to avoid dynamic allocation
    //#define maxBufferSize 29 + (8 * 8)
    uint8_t getMaxBufferSize();
    //private helper methods MiniPacket -> CanPacket

    //returns either the ID field in the CAN_message_t or the byte address in the 8 byte buffer
    int8_t getBufferIndex();

    //size of the current CAN_message_t buffer we're writing to. Can be 29, 11, or 8.
    uint8_t getBufferSize();

    //This is the number of unused bits in our current low level buffer
    uint8_t getBufferFreeSpace(); //corresponds to index of leftmost free bit

    uint8_t getBitBoundaryIndex(uint8_t bitWidth);

    /*
    data is the data we're writing to the low level buffer.
    dataWidth is how many bits data contains.
    dataOffset is how far shifted left the relevant bits are
    */
    void writeBitsHelper(uint32_t data, uint8_t dataWidth, uint8_t dataOffset); //atomic write

    /*
    This method writes to the CAN message as if it were a continuous bit-buffer.
    In practice this means that it should be easy to write data to it, even if
    that means writing some if the bits into the ID field, and other parts into
    buf[someIndex].
    data: contains what we're writing to the CAN message
    dataWidth: tells method how many bits are relevant.
    Store relevant bits starting at LSB moving to MSB for proper function.
    */

    //private helper method CAN_message_t -> AbstractedCanPacket

    //reads bitWidth bits from msg and returns them. Also increments usedBits.
    uint32_t readBitsHelper(uint8_t bitWidth, uint8_t offset);

    void writeToCAN();

    void printCanMessage();
    void printBits(int data, int size);
};

#endif
