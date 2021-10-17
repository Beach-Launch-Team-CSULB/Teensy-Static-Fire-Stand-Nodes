// -------------------------------------------------------------
// Hello World CAN send test
// by Jacob Waters
//
// This test transmits a sample CAN frame over teensy CAN0
// teensy pin 3 -> CAN TX
// teensy pin 4 _> CAN RX
// CANL -> CANL
// CANH -> CANH
// GND -> GND I.E. connect grounds of CAN Nodes

#include <FlexCAN.h>

#ifndef __MK66FX1M0__
#error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

int busSpeed = 500000; //baudrate
bool canBus = 0;       //use 0 CAN0 or 1 for CAN1
FlexCAN CANbus0(busSpeed, canBus);

static CAN_message_t msg;

// -------------------------------------------------------------
void setup(void)
{
  CANbus0.begin();

  delay(1000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));

  /*
    msg.ext=0 -> 11 Bit ID field size, maximum size = 2^11 -1 = 2047
    msg.ext=1 -> 29 Bit ID field size, maximum size = 2^29 -1 = some big number use a calculator bruh
    Note that values larger than the respective max sizes will result in overflow
  */
  msg.ext = 1;
  msg.id = 2048; //will overflow to 0 in regular ID mode,
                 //but not in extended ID Mode

  msg.timeout = 1000; //How long to keep trying before message failure in milliseconds

  msg.len = 8; //length in bytes of message, up to 8 bytes

  msg.buf[0] = (uint8_t)0x0A; //assigning hex values to bytes, can use decimal too
  msg.buf[1] = (uint8_t)0x0B;
  msg.buf[2] = (uint8_t)0x0C;
  msg.buf[3] = (uint8_t)0x00;
  msg.buf[4] = (uint8_t)0x00;
  msg.buf[5] = (uint8_t)0x01;
  msg.buf[6] = (uint8_t)0x02;
  msg.buf[7] = (uint8_t)0x03;
}

// -------------------------------------------------------------
void loop(void)
{
  //CANbus0.write returns a 1 if successful and zero if not.
  bool writeSuccessful = CANbus0.write(msg);
  if (writeSuccessful)
    Serial.println("write successful!");
  else
    Serial.println("Write failed, check hardware config");

  delay(1000);
  /*On a pi, if msg.ext == 0, should output:

    can0   000   [8]    0A 0B 0C 00 00 01 02 03  
    bus  | ID  | size | data in HEX format
    
  else if msg.ext == 1, should output:

    can0   00000800   [8]    0A 0B 0C 00 00 01 02 03
    bus  | ID       | size | data in HEX format
*/
}