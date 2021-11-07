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

    while (!Serial)
        ; //wait for serial bus to be available

    Serial.println(F("Teensy CAN Speed test WRITE"));

    msg.ext = 1;
    msg.id = 0x666; //will overflow to 0 in regular ID mode,
                   //but not in extended ID Mode

    msg.timeout = 1000; //How long to keep trying before message failure in milliseconds

    msg.len = 8; //length in bytes of message, up to 8 bytes
   for (int i = 0; i < msg.len; i++)
    {
      msg.buf[i] = i;
    }
}

// -------------------------------------------------------------
void loop(void)
{
    bool writeSuccessful;
    for (int i = 0; i < 50000; i++)
    {
        //if(CANbus0.available())
        //{
        writeSuccessful = CANbus0.write(msg);
        if (!writeSuccessful)
            Serial.println("Write failed, check hardware config");
        //}
    }
    
    while (1) //wait forever
        delay(1000);
}