#include <Arduino.h>
#include <Streaming.h>
#include "AbstractedCanPacket.h"
#include "CanBitBuffer.h"

//useful for testing and debug
void printBits2(int data, int size)
{
  for (int i = size - 1; i >= 0; i--)
  {
    Serial.print(bitRead(data, i));
    if (i % 4 == 0)
      Serial.print("");
  }
  Serial.print("-");
}

void setup()
{
  while (!Serial)
  {
    // wait for serial port to connect.
  }
}
AbstractedCanPacket myCanPacket;

CanBitBuffer canBitBufferTest;

void loop()
{
  canBitBufferTest.setExtendedID(1);
  uint32_t mask = (1 << 3) - 1; //build mask with resolution bits
  mask = mask << 1;

  Serial << "Begin CanBitBufferTest \nWrite:\n";
  while(canBitBufferTest.canFitLowLevel(5))
  {
    canBitBufferTest.setLowLevelBufferBits(mask, 5);
    //delay(500);
    canBitBufferTest.printCanMessage();    
    Serial << "\nfreeBits: " << canBitBufferTest.getFreeBitsLowLevel() << endl;

    Serial << endl << endl;
  }
  Serial << "canBitBuffer Read:\n";

  canBitBufferTest.reset();
  while(canBitBufferTest.canFitLowLevel(5))
  {
    uint32_t tempData = canBitBufferTest.readLowLevelBits(5);
    delay(500);
    printBits2(tempData,5);
    //Serial << "\nfreeBits: " << canBitBufferTest.getFreeBitsLowLevel() << endl;

    //Serial << endl << endl;
  }


  Serial << "End BitBufferTest\n\n";


  /////////////////////////////////////////////
  //while (1)
    //delay(1000);

  //refer to AbstractedCanPacket.h to see how large nodeID and messagePriority can be
  Serial << "setMessagePriority sucess: " << myCanPacket.setMessagePriority(7); //111
  Serial << " message priority: " << myCanPacket.getMessagePriority() << endl;
  Serial << "setNodeID success: " << myCanPacket.setNodeID(31); //11111
  Serial << " nodeID: " << myCanPacket.getNodeID() << endl;
  myCanPacket.setExtendedID(0);

  Serial << "free bits High Level: " << myCanPacket.getFreeBitsHighLevel() << endl;
  Serial << "free bits LowLevel: " << myCanPacket.getFreeBitsLowLevel() << endl;

  MiniPacket a;
  a.setDataLength(5);
  a.setID_Length(3);
  a.setData(0);
  a.setID(1);
  Serial << "a: ";
  a.print();
  Serial << endl
         << "a size: " << a.getSize() << endl;

  Serial << "canFitHighLevel(a): " << myCanPacket.canFitHighLevel(a) << endl;
  delay(50);

  Serial << "free bits High Level: " << myCanPacket.getFreeBitsHighLevel() << endl;
  Serial << "free bits LowLevel: " << myCanPacket.getFreeBitsLowLevel() << endl;
  while (myCanPacket.canFitHighLevel(a))
  {
    myCanPacket.highLevelAdd(a);
    //Serial << "highLevelFreeBits: " << myCanPacket.getFreeBitsHighLevel() << endl;
    //delay(50);

    a.setData(a.getData() + 1);
  }

  MiniPacket *myBuffer = myCanPacket.getPacketBuffer();

  for (int i = 0; i < myCanPacket.getBufferSize(); i++)
  {
    Serial << "myBuffer[" << i << "]: ";
    myBuffer[i].print();
    Serial << endl;
    delay(50);
  }
  Serial << "free bits High Level: " << myCanPacket.getFreeBitsHighLevel() << endl;
  Serial << "free bits LowLevel: " << myCanPacket.getFreeBitsLowLevel() << endl;
  myCanPacket.writeToCAN();
  Serial << "After write to can:" << endl;
  Serial << "msg len: " << myCanPacket.getCanMessage().len << endl;
  myCanPacket.printCanMessage();

  Serial << endl
         << "free bits High Level: " << myCanPacket.getFreeBitsHighLevel() << endl;
  Serial << "free bits LowLevel: " << myCanPacket.getFreeBitsLowLevel() << endl;

  Serial << "Original CAN Frame: ";
  myCanPacket.printCanMessage();
  Serial << endl;

  ////////////////////////////////////////////

  CAN_message_t shallowCopy = myCanPacket.getCanMessage();
  AbstractedCanPacket fromCanPacket(a.getID_Length(), shallowCopy);

  Serial << "fromCan Message Priority: " << fromCanPacket.getMessagePriority() << endl;
  Serial << "fromCan nodeID: " << fromCanPacket.getNodeID() << endl;

  MiniPacket *fromCanBuffer = fromCanPacket.getPacketBuffer();

  for (int i = 0; i < fromCanPacket.getBufferSize(); i++)
  {
    Serial << "myBuffer[" << i << "]: ";
    fromCanBuffer[i].print();
    Serial << endl;
    delay(100);
  }
  fromCanPacket.writeToCAN();
  fromCanPacket.printCanMessage();

  /*
  Serial << endl
         << "a size: " << a.getSize() << endl;
  Serial << "canFit(a): " << myCanPacket.canFitHighLevel(a) << endl;

  
  uint32_t mask = (1 << 3) - 1; //build mask with resolution bits
  mask = mask << 1;
  Serial << "mask: ";
  Serial.println(mask, BIN); ///01110
  myCanPacket.setExtendedID(false);
  //Serial << "get extended ID: " << myCanPacket.getExtendedID() << endl;
  Serial << "bit boundary index: " << myCanPacket.getLowLevelBufferFreeSpace() << endl;
*/
  //myCanPacket.setLowLevelBufferBits(mask, 5);

  //builds a = {010, 01110}
  //uint32_t mask = (1 << 3) - 1; //build mask with resolution bits
  //mask = mask << 1;
  //b.setData(mask); //01110
  //b.setID(10);     //01010
  /*
  mask = (1 << 1) - 1; //build mask with resolution bits
  mask = mask << 1;
  a.setID(mask); //010
  Serial << "a: ";
  a.print();
  Serial << endl << endl;
  */

  /*
  CAN_message_t shallowCopy = myCanPacket.getCanMessage();

  AbstractedCanPacket fromCanPacket(standardID_Length, shallowCopy);

  Serial << "fromCan Message Priority: " << fromCanPacket.getMessagePriority();

  MiniPacket *myBufferB = fromCanPacket.getPacketBuffer();

  for (int i = 0; i < fromCanPacket.getBufferSize(); i++)
  {
    Serial << "myBuffer[" << i << "]: ";
    myBufferB[i].print();
    Serial << endl;
    delay(100);
  }
  
  printCanMessage(myCanPacket.getCanMessage());
  Serial << "BitRead:" << endl;
  delay(1500);
  myCanPacket.reset();
  uint8_t bitWidth = 12;

  while (myCanPacket.canFit(bitWidth))
  {
    uint32_t data = myCanPacket.readLowLevelBits(bitWidth);
    printBits(data,bitWidth);
    delay(1500);
  }
  */
  while (1)
  {
    delay(1000);
  }
  Serial << endl
         << endl;
  delay(1500);
}
