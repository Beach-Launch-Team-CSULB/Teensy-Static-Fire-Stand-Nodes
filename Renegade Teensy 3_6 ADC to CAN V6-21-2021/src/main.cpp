// -------------------------------------------------------------
// CANtest for Teensy 3.6 dual CAN bus
// by Pawelsky (based on CANtest by teachop)
//
// This test transmits all data coming from CAN0 to CAN1 and vice versa (at 1Mbps)
//

#include <FlexCAN.h>

#ifndef __MK66FX1M0__
#error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

FlexCAN CANbus0(500000, 0);
FlexCAN CANbus1(1000000, 1);

static CAN_message_t msg;
static uint8_t hex[17] = "0123456789abcdef";

// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while (dumpLen--)
  {
    working = *bytePtr++;
    Serial.write(hex[working >> 4]);
    Serial.write(hex[working & 15]);
  }
  Serial.write('\r');
  Serial.write('\n');
}

// -------------------------------------------------------------
void setup(void)
{
  CANbus0.begin();
  CANbus1.begin();

  delay(1000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));
  msg.id = 7;
  msg.len = 1;
  msg.timeout = 1000;
  msg.buf[0] = (uint8_t)0xFF;
}

// -------------------------------------------------------------
void loop(void)
{
  Serial.println(CANbus0.write(msg));
  delay(1000);
}