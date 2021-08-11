// -------------------------------------------------------------
// Can READ test
// by Jacob Waters
//
// This test listens for a sample CAN frame over teensy 3.6 CAN0
// teensy pin 3 -> CAN TX
// teensy pin 4 _> CAN RX
// CANL -> CANL
// CANH -> CANH
// GND -> GND I.E. connect grounds of CAN Nodes

#include <Arduino.h>
#include <FlexCAN.h>

int busSpeed = 500000; //baudrate
bool canBus = 0;       //use 0 CAN0 or 1 for CAN1
FlexCAN CANbus0(busSpeed, canBus);

static CAN_message_t msg;

// -------------------------------------------------------------
void setup(void)
{
  CANbus0.begin();

  delay(1000);
  Serial.println(F("Teensy 3.6 can bus READ test"));
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
void loop(void)
{
  delay(1000);
  Serial.println(CANbus0.available());
  if (CANbus0.available())
  {
    CANbus0.read(msg);

    Serial.print("CAN bus 0: ");
    printData(msg);

    Serial.print("ID: ");
    Serial.println(msg.id, HEX);
  }
}