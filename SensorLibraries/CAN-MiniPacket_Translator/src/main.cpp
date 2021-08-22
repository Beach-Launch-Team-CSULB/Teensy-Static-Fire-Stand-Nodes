#include <Arduino.h>
#include <Streaming.h>
#include "AbstractedCanPacket.h"

void setup()
{
  while (!Serial)
  {
    // wait for serial port to connect.
  }
}
AbstractedCanPacket myCanPacket;
void loop()
{
  //refer to AbstractedCanPacket.h to see how large nodeID and messagePriority can be
  Serial << "setNodeID success: " << myCanPacket.setNodeID(31);
  Serial << " nodeID: " << myCanPacket.getNodeID() << endl;
  Serial << "setMessagePriority sucess: " << myCanPacket.setMessagePriority(7);
  Serial << " message priority: " << myCanPacket.getMessagePriority() << endl;
  Serial << "free bits: " << myCanPacket.getFreeBits() << endl;
  MiniPacket a;
  a.setDataLength(3);
  a.setID_Length(1);
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


  Serial << endl << endl;
  delay(10000);
}
