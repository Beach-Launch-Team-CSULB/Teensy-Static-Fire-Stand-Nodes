/*
    Author Jacob Waters
    fierywaters13@gmail.com
    Designed for CSULB Beach Launch Team
*/
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
	delay(50); //for some reason serial prints are getting cut off
	//////////////////////////////////////////NodeID
	Serial << "\n\nSetting the nodeID" << endl;
	uint8_t maxNodeID = ((1 << nodeIDLength) - 1);			//2^ID_length -1
	Serial << "max nodeID value is: " << maxNodeID << endl; //2^ID_length -1
	abstractCanPacket.setNodeID(maxNodeID);
	Serial << "nodeID is now: " << abstractCanPacket.getNodeID() << endl;

	//////////////////////////////////////////Priority
	Serial << "Setting the Priority: " << endl;
	Serial << "max Priority value is: " << ((1 << priorityLength) - 1) << endl; //2^ID_length -1
	abstractCanPacket.setPriority(2);											//010
	Serial << "Priority is now: " << abstractCanPacket.getPriority() << endl;

	delay(500);
	//////////////////////////////////////////Configuring MiniPackets
	MiniPacket sensorData;

	uint8_t idLength = 5; //5 bit ID field
	sensorData.setID_Length(idLength);

	uint8_t dataLength = 12; //12 bit ADC read
	sensorData.setDataLength(dataLength);

	uint32_t largestID = (1 << idLength) - 1;
	uint32_t largestData = (1 << dataLength) - 1;
	Serial << "Largest ID: " << largestID << endl;
	Serial << "Largest Data: " << largestData << endl;
	delay(500);

	//////////////////////////////////////////Writing to MiniPacket
	Serial << "\nWriting too large a number to data now: " << largestData + 22 << endl;
	//these statements should fail
	if (sensorData.setData(largestData + 22) == true) //if our assignment is successful
	{
		Serial << "The data was set without issue" << endl;
	}
	else
	{
		Serial << "setData() returned false and failed hard." << endl;
	}
	delay(500);

	//////////////////////////////////////////Forced Trucation - WITH GREAT POWER COMES GREAT RESPONSIBILITY
	Serial << "Forcing Truncation, do this with EXTREME CAUTION. " << endl;
	uint32_t truncatedData = sensorData.truncate(largestData + 22, dataLength);
	sensorData.setData(truncatedData);

	Serial << endl
		   << "sensorData after truncation: ";
	sensorData.print();
	Serial << endl;
	delay(500);

	//////////////////////////////////////////MiniPacket setID()
	Serial << "setID test: " << endl;
	if (sensorData.setID(0)) //Zero is reserved NULL Address
	{
		Serial << "This statement won't execute\n"; //if it does, something is wrong
	}
	else //operation failed
	{
		Serial << "setID(0) failed, setting to 21 instead" << endl;
		sensorData.setID(21);
		Serial << "After setID(21): ";
		sensorData.print();
		Serial << endl
			   << endl;
	}
	delay(500);

	//////////////////////////////////////////Adding MiniPackets to AbstractedCanPacket
	Serial << "sensorData is now ready to send." << endl;
	Serial << "We have " << abstractCanPacket.getFreeBits() << " free bits before writing anything (other than nodeID and priority)." << endl;

	if (abstractCanPacket.canFit(sensorData))
	{
		abstractCanPacket.add(sensorData);
		Serial << "Added sensorData to abstractCanPacket" << endl;
	}
	else //AbstractedCanPacket is full
	{
		Serial << "You'd normally send the abstractedCanPacket here"; //but this line won't execute anyway
	}

	Serial << "\nAdding filler MiniPackets to abstractedCanPacket until full: " << endl;
	MiniPacket filler(idLength, dataLength);
	filler.setID(1);   //remember zero ID is forbidden
	filler.setData(0); //zero data is fine

	while (abstractCanPacket.add(filler)) //add duplicates until full. Note add returns true if successful, false otherwise.
	{
		//increment so we know written data is unique
		filler.setData(filler.getData() + 1);
		filler.setID(filler.getID() + 1);
	}
	delay(500);

	//////////////////////////////////////////Viewing our MiniPacket buffer, pre-send
	Serial << "after add loop, abstractedCanPacket contains: " << endl;
	MiniPacket *packetArray = abstractCanPacket.getPacketBuffer();
	for (int i = 0; i < abstractCanPacket.getBufferSize(); i++)
	{
		Serial << "packetArray[" << i << "]: ";
		packetArray[i].print();
		Serial << endl;
	}
	Serial << endl;

	Serial << "Free space in bits after Write: " << abstractCanPacket.getFreeBits() << endl
		   << endl;
	delay(500);

	//////////////////////////////////////////Reading from CAN
	const CAN_message_t shallowCopy = abstractCanPacket.getCanMessage(); //note that array is shared (ie a shallow copy)
	//Here, you'd send the CAN Frame out the CAN Bus (This is a cheap simulation(I'm lazy))

	//ON OTHER NODE
	//Read our CAN Frame off the CAN Bus (still a sumulation)
	//wow, we are now on the other node,
	//and shallowCopy is there and what we read from the bus, look at that. Wasn't that nifty?
	Serial << "We have teleported across space and time to another micro (wow, magic)" << endl;

	Serial << "Building AbstractedCanPacket from CAN_message_t!" << endl;
	AbstractedCanPacket incomingCanPacket(idLength, shallowCopy);

	Serial << "nodeID: " << incomingCanPacket.getNodeID();
	Serial << ", Priority: " << incomingCanPacket.getPriority() << endl;

	Serial << "\nReading incomingCanPacket MiniPackets:" << endl;
	MiniPacket *incomingArray = incomingCanPacket.getPacketBuffer();
	for (int i = 0; i < incomingCanPacket.getBufferSize(); i++)
	{
		Serial << "packetArray[" << i << "]:";
		incomingArray[i].print();
		Serial << endl;
	}
	Serial << endl;

	/*
	Rather than make a new AbstractCanPacket each time, simply reset them
	so they can be reused without additional memory use. UNTESTED
	*/
	incomingCanPacket.reset();
	abstractCanPacket.reset();

	while (true)
		delay(1000); //disable loop
}
