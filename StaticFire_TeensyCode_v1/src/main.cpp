#include <Arduino.h>
#include <array>
#include <FlexCAN.h>
#include <ADC.h>
#include <ADC_util.h>
#include "ValveClass.h"
#include "ValveDefinitions.h"
#include "CANRead.h"
#include "ControlCommands.h"
#include "OperationFunctionTemplates.h"

int busSpeed = 500000; //baudrate
//bool canBus = 0;       //use 0 CAN0 or 1 for CAN1
//FlexCAN CANbus0(canBus);

int enablePin{1};

uint32_t loopCount {0};

Command currentCommand{command_NOCOMMAND};

// timer to not print too much
elapsedMillis timer;
elapsedMillis timer2;

static CAN_message_t msgIn;

void printData(CAN_message_t& msg)
{
  for (int i = 0; i < msg.len - 1; i++)
  {
    Serial.print(msg.buf[i], HEX);
    Serial.print(".");
  }
  Serial.print(msg.buf[msg.len - 1], HEX);
  Serial.println();
}

CAN_message_t msgBuild(const uint8_t& message)
{
  // builds a simple message and duplicates it 8x
  CAN_message_t msgOut;

  msgOut.ext = 1;
  msgOut.id = 2048; //will overflow to 0 in regular ID mode,
                 //but not in extended ID Mode

  //msgOut.timeout = 1000; //How long to keep trying before message failure in milliseconds

  msgOut.len = 8; //length in bytes of message, up to 8 bytes

  for(uint32_t i{0}; i < 8; ++i)
  {
    msgOut.buf[i] = message;
  }

  return msgOut;
}
// -------------------------------------------------------------

void setup() 
{
  // Enable pin write low
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);

  // Run Valve Setup
  valveSetUp(valveArray);

  // Test Valve
  // testValve.setState(ValveState::OpenCommanded);

  // Start Can
  Can0.begin(busSpeed);

  // Start Serial
  Serial.begin(115200);
  delay(5000);
  
  // for(int i{0}; i < 10; ++i)
  // {
  //   Serial.println(10-i);
  //   delay(1000);
  // }

  Serial.println("Test Begin");

  // uint8_t message {1};
  // CAN_message_t msgOut{msgBuild(message)};

  // bool writeSuccessful = Can0.write(msgOut);
  // if (writeSuccessful)
  //   Serial.println("write successful!");
  // else
  //   Serial.println("Write failed, check hardware config");

  timer = 0;
  timer2 = 0;
  digitalWrite(enablePin, HIGH);
}

void loop() 
{
  // CANtest
  if(CANread(Can0, currentCommand))
  {
    Serial.print("Command Recieved: ");
    Serial.println(currentCommand);
  }

  valveTasks(valveArray);

  switch (currentCommand)
  {
    case command_valveOpen:
      testValve.setState(ValveState::OpenCommanded);
      break;
    case command_valveClosed:
      testValve.setState(ValveState::CloseCommanded);
      break;
    default:
      break;
  }
  // {
  //   Serial.println("Command Recieved.");
  //   //Serial.println(currentCommand);
  // }
  // delay(1000);
  // Serial.println(currentCommand);
  // Serial.print("Loop Count:");
  // Serial.println(loopCount);
  //Serial.println(CANbus0.available());

  // if (Can0.available())
  // {
  //     Can0.read(msgIn);
    
  //     Serial.print("CAN bus 0: ");
  //     printData(msgIn);

  //     Serial.print("ID: ");
  //     Serial.println(msgIn.id, HEX);
    
  // }

if(timer >= 10)
  {
    uint8_t message {static_cast<uint8_t>(loopCount)};
    CAN_message_t msgOut{msgBuild(message)};
    Can0.write(msgOut);
    ++loopCount;
    timer = 0;
  }
  // if (writeSuccessful)
  //   Serial.println("write successful!");
  // else
  //   Serial.println("Write failed, check hardware config");


  

  



}