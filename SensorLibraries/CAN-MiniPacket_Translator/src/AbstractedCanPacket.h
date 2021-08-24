/*
    An abstracted CAN packet holds the priority of the CAN packet
    it represents, the Node ID, and an arbitrary number of
    MiniPackets.

    It should hold a vector of MiniPackets, as well as keep track
    of how many MiniPackets, it contains, as well as if it can fit
    any more.
*/

//IMPORTANT CONFIG #DEFINES, THIS IS ALL PEASANTS GET TO TOUCH IN HERE

#define priorityID_Length 0
#define priorityDataLength 3

#define nodeID_ID_Length 5
#define nodeID_DataLength 0

#define smallestMiniPacketSize 5 //size of smallest possible MiniPacket to be sent

#define standardID_Length 5//this needs to be known beforehand for receiving CAN messages

//IMPORTANT CONFIG #DEFINES

#include <Arduino.h>
#include <Vector.h>
#include <FlexCAN.h>
#include "MiniPacket.h"
#include "BitChopper.h"

#ifndef AbstractedCanPacker_H
#define AbstractedCanPacker_H

class AbstractedCanPacket
{
public:
    AbstractedCanPacket();

    //CAN frame config
    void setExtendedID(bool extID);
    bool getExtendedID();

    //for converting MiniPackets to CAN_Frames
    AbstractedCanPacket(uint8_t idLength);
    bool setMessagePriority(uint32_t priority);
    bool setNodeID(uint32_t ID);
    uint8_t getFreeBits();              //returns free space in bits
    bool canFit(MiniPacket nextPacket); //returns true if it can fit the MiniPacket
    bool add(MiniPacket next);          //returns true if add successful
    bool send();                        //returns true if send successful, false otherwise

    //for converting CAN Frames to MiniPackets
    AbstractedCanPacket(uint8_t idLength, CAN_message_t CAN_Message);
    MiniPacket *getPacketBuffer();
    uint8_t getBufferSize();
    uint32_t getMessagePriority();
    uint32_t getNodeID();

    void reset();//resets can packet to be reused. However, old data is not deleted.

private:
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

    uint8_t usedBits;      //current size of abstracted bit buffer

    //payload data
    MiniPacket priority; //info contained in data field
    MiniPacket nodeID;   //info contained in ID field

    uint8_t index;

//compiler pre-processing to figure out how big out array needs to be, in order to avoid dynamic allocation
#define overheadBits (priorityID_Length + priorityDataLength + nodeID_ID_Length + nodeID_DataLength)
#define maxBufferSize (29 + (8 * 8) - overheadBits)
#define maxWeCouldStore (maxBufferSize / smallestMiniPacketSize)
    MiniPacket packetBuffer[maxWeCouldStore]; //our array

    public://TESTING, CHANGE THIS TO PRIVATE LATER
    //private helper methods MiniPacket -> CanPacket

    //returns either the ID field in the CAN_message_t or the byte address in the 8 byte buffer
    int8_t getLowLevelBufferIndex();
    //size of the current CAN_message_t buffer we're writing to. Can be 29, 11, or 8.
    uint8_t getLowLevelBufferSize();
    //This is the number of unused bits in our current low level buffer
    uint8_t getLowLevelBufferFreeSpace(); //corresponds to index of leftmost free bit
    uint8_t getLowLevelBitBoundaryIndex(uint8_t bitWidth);
    /*
    data is the data we're writing to the low level buffer.
    dataWidth is how many bits data contains.
    dataOffset is how far shifted left the relevant bits are
    */
    void setLowLevelBufferBitsHelper(uint32_t data, uint8_t dataWidth, uint8_t dataOffset);//atomic write

    /*
    This method writes to the CAN message as if it were a continuous bit-buffer.
    In practice this means that it should be easy to write data to it, even if
    that means writing some if the bits into the ID field, and other parts into
    buf[someIndex].
    data: contains what we're writing to the CAN message
    dataWidth: tells method how many bits are relevant.
    Store relevant bits starting at LSB moving to MSB for proper function.
    */
    void setLowLevelBufferBits(uint32_t data, uint8_t dataWidth);

    /*
    public add function will update high level MiniPacket buffer, this function
    will update the private CAN_message_t within it. Also, this method updates
    usedBits.
    */
    void lowLevelAdd(MiniPacket nextPacket);

    //private helper method CAN_message_t -> AbstractedCanPacket

    //reads bitWidth bits from msg and returns them. Also increments usedBits.
    uint32_t readLowLevelBitsHelper(uint8_t bitWidth, uint8_t offset);
    uint32_t readLowLevelBits(uint8_t bitWidth);

    CAN_message_t getCanMessage();//testing REMOVE THIS. Returns the private CAN message this class encodes
    void setUsedBits();
};

#endif
