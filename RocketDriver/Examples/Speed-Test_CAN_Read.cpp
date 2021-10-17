// -------------------------------------------------------------
// CAN Read Speed test
// by Jacob Waters
//
// This is a simple speed test for receiving CAN packets on a teensy 3.6
// teensy pin 3 -> CAN TX
// teensy pin 4 _> CAN RX
// CANL -> CANL
// CANH -> CANH
// GND -> GND I.E. connect grounds of CAN Nodes

#include <Arduino.h>
#include <FlexCAN.h>
#include <Streaming.h>

int busSpeed = 500000; //baudrate
bool canBus = 0;       //use 0 CAN0 or 1 for CAN1
FlexCAN CANbus0(busSpeed, canBus);

static CAN_message_t msg;

uint32_t canCount = 0;
uint32_t startTime, lastReceivedTime;
// -------------------------------------------------------------
void setup(void)
{
  while (!Serial)
    ; //wait for serial to connect
  CANbus0.begin();

  Serial.println(F("Teensy 3.6 can bus SPEED test"));

  startTime = millis();
  lastReceivedTime = millis();
}

// -------------------------------------------------------------
void printData(CAN_message_t &msg)
{
  for (int i = 0; i < msg.len - 1; i++)
  {
    Serial.print(msg.buf[i], HEX);
    Serial.print(".");
  }
  Serial.print(msg.buf[msg.len - 1], HEX);
  Serial.println();
}
// -------------------------------------------------------------
void putBreakPointHere() {}
void loop(void)
{
  //delay(1000);
  //Serial.println(CANbus0.available());
  if (millis() - lastReceivedTime > 100)
  {
    if (canCount > 0)
    {
      double secondsElapsed = (millis() - startTime) / 1000.0;
      Serial << "canCount: " << canCount << endl;
      Serial << "seconds: " << secondsElapsed << endl;
      double framesPerSecond = canCount / secondsElapsed;
      Serial << "Frames/S: " << framesPerSecond << endl;
      Serial << "effectiveBitrate: " << (64+31)*framesPerSecond << endl << endl;
      Serial << "Teensy 3.6 can bus SPEED test" << endl;


      canCount = 0;
    }

    startTime = millis();
  }

  if (CANbus0.available())
  {
    CANbus0.read(msg);
    lastReceivedTime = millis();
    canCount++;
    //Serial.print("CAN bus 0: ");
    //printData(msg);
    if(msg.id != 0x666)
      Serial << "ERROR, ID Doesn't match" << endl;
    bool copyError = false;
    for(int i =0; i<msg.len;i++)
      if(msg.buf[i]!=128)
        copyError = true;
    if(copyError)
      Serial << "ERROR IN DATA" << endl;
    //Serial.print("ID: ");
    //Serial.println(msg.id, HEX);
  }
}