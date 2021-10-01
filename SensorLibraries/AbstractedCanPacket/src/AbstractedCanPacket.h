/*
    Author Jacob Waters
    fierywaters13@gmail.com
    Designed for CSULB Beach Launch Team
*/
/*
    An abstracted CAN packet holds the priority of the CAN packet
    it represents, the Node ID, and an arbitrary number of
    MiniPackets.

    It should hold a vector of MiniPackets, as well as keep track
    of how many MiniPackets, it contains, as well as if it can fit
    any more.
*/

//IMPORTANT CONFIG #DEFINES, THIS IS ALL PEASANTS GET TO TOUCH IN HERE

#define priorityLength 3

#define nodeIDLength 3

#define smallestMiniPacketSize 5 //size of smallest possible MiniPacket to be sent

//#define standardID_Length 5 //this needs to be known beforehand for receiving CAN messages

//IMPORTANT CONFIG #DEFINES

#include <Arduino.h>
#include <FlexCAN.h>
#include "MiniPacket.h"
#include "BitChopper.h"
#include "CanBitBuffer.h"

#ifndef AbstractedCanPacker_H
#define AbstractedCanPacker_H

class AbstractedCanPacket
{
public:
    AbstractedCanPacket();
    void init();

    //CAN frame config
    void setExtendedID(bool extID);
    bool getExtendedID();

    //for converting MiniPackets to CAN_Frames
    bool setPriority(uint32_t priority);
    bool setNodeID(uint32_t ID);
    uint8_t getFreeBits();              //returns free space in bits
    bool canFit(MiniPacket nextPacket); //returns true if it can fit the MiniPacket
    bool add(MiniPacket next);          //returns true if add successful
    CAN_message_t getCanMessage();

    //for converting CAN Frames to MiniPackets
    AbstractedCanPacket(uint8_t idLength, CAN_message_t CAN_Message);
    MiniPacket *getPacketBuffer();
    uint8_t getBufferSize();
    uint32_t getPriority();
    uint32_t getNodeID();

    void reset();           //resets CAN packet to be reused. However, old data is not deleted.
    void printCanMessage(); //in binary format

private:
    //PRIVATE HELPER METHODS
    //MiniPacket ->CAN Frame
    bool canFit(uint8_t nBits);                 //returns true if it can fit nBits more bits. Should be private
    bool canFitLowLevel(uint8_t nBits);         //returns true if it can fit nBits more bits into the CanBitBuffer. Should be private
    bool canFitLowLevel(MiniPacket nextPacket); //returns true if it can fit the MiniPacket
    uint8_t getMaxBufferSize();
    /*
    public add function will update high level MiniPacket buffer, this function
    will update the private CAN_message_t within it. Also, this method updates
    usedBits.
    */
    bool addLowLevel(MiniPacket nextPacket);
    void writeToCAN();

    //private:
    CanBitBuffer msg;

    //payload data
    uint32_t nodeID;   //info contained in ID field
    uint32_t priority; //info contained in data field
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

    uint8_t usedBits; //current size of the high-level bit buffer

    /*
    This keeps track of the size of packetBuffer.
    It is also conveniently the next index to write to when adding an additional
    MiniPacket to the packetBuffer, hence its name.
    */
    uint8_t packetBufferSize;

//compiler pre-processing to figure out how big out array needs to be, in order to avoid dynamic allocation
#define overheadBits (priorityLength + nodeIDLength)
#define maxPossibleBufferSize 29 + (8 * 8)
#define maxWeCouldStore (maxPossibleBufferSize / smallestMiniPacketSize)
    MiniPacket packetBuffer[maxWeCouldStore]; //our array

    //private helper method CAN_message_t -> AbstractedCanPacket
    MiniPacket read(uint8_t ID_Length, uint8_t dataLength);
    MiniPacket read(uint8_t ID_Length);
    uint8_t packetIdToDataLength(uint8_t nodeID, uint8_t packetId); //function mapping for variable data lengths for minipacket
};

#endif
