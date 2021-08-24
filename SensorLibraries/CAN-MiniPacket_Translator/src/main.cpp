#include <Arduino.h>
#include <Streaming.h>
#include "AbstractedCanPacket.h"
//#include "BitChopper.h"

void setup()
{
  while (!Serial)
  {
    // wait for serial port to connect.
  }
}
AbstractedCanPacket myCanPacket;

//useful for testing and debug
void printBits(int data, int size)
{
  for (int i = size - 1; i >= 0; i--)
  {
    Serial.print(bitRead(data, i));
    if (i % 4 == 0)
      Serial.print("");
  }
  Serial.print("");
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
  printBits(msg.id, idSize);
  for (int i = 0; i < msg.len; i++)
  {
    printBits(msg.buf[i], 8);
  }
}
void loop()
{
  /*
  //refer to AbstractedCanPacket.h to see how large nodeID and messagePriority can be
  Serial << "setNodeID success: " << myCanPacket.setNodeID(31);
  Serial << " nodeID: " << myCanPacket.getNodeID() << endl;
  Serial << "setMessagePriority sucess: " << myCanPacket.setMessagePriority(7);
  Serial << " message priority: " << myCanPacket.getMessagePriority() << endl;
  Serial << "free bits: " << myCanPacket.getFreeBits() << endl;
  MiniPacket a;
  a.setDataLength(3);
  a.setID_Length(2);
  a.setData(0);
  a.setID(1);
  Serial << "a: ";
  a.print();
  Serial << endl << "a size: " << a.getSize() << endl;

  Serial << "canFit(a): " <<  myCanPacket.canFit(a) << endl;

  while (myCanPacket.canFit(a))
  {
    myCanPacket.add(a);
    a.setData(a.getData()+1);
  }

  MiniPacket * myBuffer = myCanPacket.getPacketBuffer();

  for(int i =0; i < myCanPacket.getBufferSize(); i++)
  {
    Serial << "myBuffer[" << i << "]: ";
    myBuffer[i].print();
    Serial << endl;
  }

  Serial << "free bits: " << myCanPacket.getFreeBits() << endl;
  Serial << endl << "a size: " << a.getSize() << endl;
  Serial << "canFit(a): " <<  myCanPacket.canFit(a) << endl;
*/

  /*
  uint32_t mask = (1 << 3) - 1; //build mask with resolution bits
  mask = mask << 1;
  Serial << "mask: ";
  Serial.println(mask, BIN); ///01110
  myCanPacket.setExtendedID(false);
  //Serial << "get extended ID: " << myCanPacket.getExtendedID() << endl;
  Serial << "bit boundary index: " << myCanPacket.getLowLevelBufferFreeSpace() << endl;
*/
  //myCanPacket.setLowLevelBufferBits(mask, 5);

  MiniPacket a;
  a.setDataLength(5);
  a.setID_Length(0);

 //builds a = {010, 01110}
  uint32_t mask = (1 << 3) - 1; //build mask with resolution bits
  mask = mask << 1;
  a.setData(mask);     //01110
  /*
  mask = (1 << 1) - 1; //build mask with resolution bits
  mask = mask << 1;
  a.setID(mask); //010
  Serial << "a: ";
  a.print();
  Serial << endl << endl;
  */


  //int counter = 0;
  while (myCanPacket.canFit(a))
  {
    //a.setData(counter << 1);
    //counter++;

    myCanPacket.add(a);
    printCanMessage(myCanPacket.getCanMessage());
    Serial << endl;
    delay(150);
  }
  printCanMessage(myCanPacket.getCanMessage());
  Serial << "BitRead:" << endl;
  delay(1500);
  myCanPacket.reset();
  while (1)
  {
    uint8_t bitWidth = 12;
    uint32_t data = myCanPacket.readLowLevelBits(bitWidth);
    printBits(data,bitWidth);
    delay(1500);
  }
  Serial << endl
         << endl;
  delay(1500);
}
