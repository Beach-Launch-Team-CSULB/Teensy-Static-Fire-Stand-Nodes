#include <Arduino.h>
#include <Streaming.h>
#include "AbstractedCanPacket.h"
#include "CanBitBuffer.h"

void waitHere()
{
  while (1)
    delay(1000);
}
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

void printCanMessage(CAN_message_t msg)
{
  uint8_t idSize;
  if (msg.ext)
  {
    idSize = 29;
  }
  else
  {
    idSize = 11;
  }
  printBits2(msg.id, idSize);
  for (int i = 0; i < msg.len; i++)
  {
    printBits2(msg.buf[i], 8);
  }
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

  /*
  /////////////////////////////////////////////
  canBitBufferTest.setExtendedID(1);
  uint32_t mask = (1 << 3) - 1; //build mask with resolution bits
  mask = mask << 1;

  Serial << "\nfreeBits: " << canBitBufferTest.getFreeBits() << endl;
  Serial << "Begin CanBitBufferTest \nWrite:\n";

  while (canBitBufferTest.canFit(5))
  {
    canBitBufferTest.writeBits(mask, 5);
    //delay(500);
    canBitBufferTest.printCanMessage();
    Serial << "\nfreeBits: " << canBitBufferTest.getFreeBits() << endl;

    Serial << endl
           << endl;
  }
  Serial << "canBitBuffer Read:\n";

  canBitBufferTest.reset();
  while (canBitBufferTest.canFit(5))
  {
    uint32_t tempData = canBitBufferTest.readBits(5);
    delay(500);
    printBits2(tempData, 5);
    //Serial << "\nfreeBits: " << canBitBufferTest.getFreeBits() << endl;

    //Serial << endl << endl;
  }

  Serial << "\nEnd BitBufferTest\n\n";
  while (1)
    delay(1000);
  //*/

  /////////////////////////////////////////////
  /*/
  //refer to AbstractedCanPacket.h to see how large nodeID and messagePriority can be
  Serial << "setMessagePriority sucess: " << myCanPacket.setMessagePriority(7); //111
  Serial << " message priority: " << myCanPacket.getMessagePriority() << endl;
  Serial << "setNodeID success: " << myCanPacket.setNodeID(31); //11111
  Serial << " nodeID: " << myCanPacket.getNodeID() << endl;
  Serial << "free bits High Level: " << myCanPacket.getFreeBits() << endl;

  myCanPacket.setExtendedID(true);

  Serial << "free bits High Level: " << myCanPacket.getFreeBits() << endl;
  //Serial << "free bits LowLevel: " << myCanPacket.getFreeBitsLowLevel() << endl;
  Serial << "usedBits LowLevel: " << myCanPacket.msg.usedBits << endl;
  Serial << "extID: " << myCanPacket.msg.msg.ext << endl;

  MiniPacket a;
  a.setDataLength(5);
  a.setID_Length(4);
  a.setData(0);
  a.setID(1);
  Serial << "a: ";
  a.print();
  Serial << endl
         << "a size: " << a.getSize() << endl;

  Serial << "canFit(a): " << myCanPacket.canFit(a) << endl;
  delay(50);

  Serial << "free bits High Level: " << myCanPacket.getFreeBits() << endl;
  //Serial << "free bits LowLevel: " << myCanPacket.getFreeBitsLowLevel() << endl;
  while (myCanPacket.canFit(a))
  {
    myCanPacket.add(a);
    Serial << "highLevelFreeBits: " << myCanPacket.getFreeBits() << endl;
    delay(50);

    a.setData(a.getData() + 1);
    a.setID((a.getID() +1));
  }

  MiniPacket *myBuffer = myCanPacket.getPacketBuffer();

  for (int i = 0; i < myCanPacket.getBufferSize(); i++)
  {
    Serial << "myBuffer[" << i << "]: ";
    myBuffer[i].print();
    Serial << endl;
    delay(50);
  }
  Serial << "free bits High Level: " << myCanPacket.getFreeBits() << endl;
  //Serial << "free bits LowLevel: " << myCanPacket.getFreeBitsLowLevel() << endl;
  myCanPacket.writeToCAN();
  Serial << "After write to can:" << endl;
  Serial << "msg len: " << myCanPacket.msg.msg.len << endl;
  myCanPacket.msg.printCanMessage();
  Serial << endl;

  // Serial << endl
  //        << "free bits High Level: " << myCanPacket.getFreeBits() << endl;
  //Serial << "free bits LowLevel: " << myCanPacket.getFreeBitsLowLevel() << endl;
  //*/
  ////////////////////////////////////////////
  
  /*/
  Serial << "Begin CAN Frame -> AbstractedCanPacket TEST\nCopied CAN Frame:";
  CAN_message_t shallowCopy = myCanPacket.getCanMessage();
  printCanMessage(shallowCopy);
  Serial << endl;
      Serial << "shallowCopy extID: " << shallowCopy.ext << endl;


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
//*/
  /*
  Serial << endl
         << "a size: " << a.getSize() << endl;
  Serial << "canFit(a): " << myCanPacket.canFit(a) << endl;

  
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
 waitHere();
}
