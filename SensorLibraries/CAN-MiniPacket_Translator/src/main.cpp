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


void printBits(int data, int size)
{
  for(int i = size-1; i >= 0; i--)
  {
    Serial.print(bitRead(data,i));
    if(i % 4 == 0) Serial.print("");
  }  
  Serial.print("-");
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
  uint32_t mask = (1 << 3) - 1; //build mask with resolution bits
  mask = mask << 1;
  Serial << "mask: ";
  Serial.println(mask, BIN); ///01110
  myCanPacket.setExtendedID(false);
  //Serial << "get extended ID: " << myCanPacket.getExtendedID() << endl;
  Serial << "bit boundary index: " << myCanPacket.getLowLevelBufferFreeSpace() << endl;

  myCanPacket.setLowLevelBufferBits(mask, 5);
  //Serial.print(myCanPacket.getCanMessage().id, BIN);
  printBits(myCanPacket.getCanMessage().id,11);
  for (int i = 0; i < 8; i++)
  {
    printBits(myCanPacket.getCanMessage().buf[i], 8);
    //Serial << " ";
    //Serial.print(myCanPacket.getCanMessage().buf[i], BIN);
  }
  Serial << endl;

  Serial << endl
         << endl;
  delay(1500);
}
