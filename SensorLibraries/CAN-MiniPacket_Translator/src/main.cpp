#include <Arduino.h>
#include <Streaming.h>
#include "AbstractedCanPacket.h"
#include "CanBitBuffer.h"

void setup()
{
    while (!Serial)
    {
        // wait for serial port to connect.
    }
}
AbstractedCanPacket abstractCanPacket;

void loop()
{
    Serial << "Setting the nodeID" << endl;
    Serial << "max nodeID value is: " << ((1 << nodeID_ID_Length) -1); //2^ID_length -1
    abstractCanPacket.setNodeID(0);

    MiniPacket sensorData;

    uint8_t idLength = 5; //5 bit ID field
    sensorData.setID_Length(idLength);

    uint8_t dataLength = 12; //12 bit ADC read
    sensorData.setDataLength(dataLength);

    uint32_t largestID = (1 << idLength) - 1;
    uint32_t largestData = (1 << dataLength) - 1;
    Serial << "Largest ID: " << largestID << endl;
    Serial << "Largest Data: " << largestData << endl;

    Serial << "\nWriting too large a number to data now: " << largestData + 22 << endl;
    //these statements should fail
    if (sensorData.setData(largestData + 22) == true) //if our assignment is successful
    {
        Serial << "The data was set without issue" << endl;
    }
    else
    {
        Serial << "set data returned false and failed hard." << endl;
    }

    //forcing truncation in MiniPacket
    uint32_t truncatedData = sensorData.truncate(largestData + 22, dataLength);
    sensorData.setData(truncatedData);

    Serial << endl
           << "sensorData after truncation: ";
    sensorData.print();
    Serial << endl;

    Serial << "setID test: " << endl;
    if (sensorData.setID(0)) //Zero is reserved NULL Address
    {
        Serial << "This shouldn't work\n";
    }
    else
    {
        Serial << "setting to 21 instead" << endl;
        sensorData.setID(21);
    }
    Serial << "After setID: ";
    sensorData.print();
    Serial << endl;

    Serial << "sensorData is now ready to send." << endl;
    if (abstractCanPacket.canFit(sensorData))
    {
        abstractCanPacket.add(sensorData);
        Serial << "Added sensorData to abstractCanPacket" << endl;
    }
    else //AbstractedCanPacket is full
    {
        Serial << "You'd normally send the abstractedCanPacket here";
    }

    Serial << "\nAdding sensorData to abstractedCanPacket until full: " << endl;
    MiniPacket filler(idLength, dataLength);
    filler.setID(1);   //remember zero ID is forbidden
    filler.setData(0); //zero data is fine

    while (abstractCanPacket.canFit(filler)) //add duplicates until full
    {
        abstractCanPacket.add(filler);

        //increment so we know written data is unique
        filler.setData(filler.getData() + 1);
        filler.setID(filler.getID() + 1);
    }

    Serial << "after add loop, abstractedCanPacket contains: " << endl;
    MiniPacket *packetArray = abstractCanPacket.getPacketBuffer();
    for (int i = 0; i < abstractCanPacket.getBufferSize(); i++)
    {
        Serial << "packetArray[" << i << "]:";
        packetArray[i].print();
        Serial << endl;
    }
    Serial << endl;

    Serial << "\nFree space in bits: " << abstractCanPacket.getFreeBits() << endl;

    abstractCanPacket.setMessagePriority(7);

    while (true)
    {
        delay(1000);
    }
}
