#include "AbstractedCanPacket.h"

AbstractedCanPacket::AbstractedCanPacket()
{
    ID_Length = 5; //TESTING! Configure for implementation specific address size

//////////////////////////////////////////Configure overhead bits
    priority.ID_Length = 0;
    priority.dataLength = 3;

    nodeID.ID_Length = 5;
    nodeID.data = 0;

    uint8_t overheadBits = priority.ID_Length + priority.dataLength + nodeID.ID_Length + nodeID.data;

    maxBufferSize = 29 + (8 * 8) - overheadBits;

//////////////////////////////////////////initialize our array
uint8_t smallestMiniPacketSize = 5;//bits
uint8_t maxWeCouldStore = maxBufferSize/smallestMiniPacketSize;
MiniPacket storageArray[maxWeCouldStore];
//packetBuffer.setStorage(storageArray);

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
}