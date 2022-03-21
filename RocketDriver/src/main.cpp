// RocketDriver Propulsion Control and Data Acquisition - Embedded System Node Program
// Originally by Dan Morgan and Mat Arnold
// For Renegade, Pasafire, Beach Launch Team, and more
//
//

#include <Arduino.h>
#include <EEPROM.h>
#include <ADC.h>
#include <ADC_util.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <WireKinetis.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include <InternalTemperature.h>
#include <array>
#include <string>
#include <list>
using std::string;

#include "CANRead.h"
#include "CANWrite.h"
#include "CANReports.h"
#include "OperationFunctionTemplates.h"
#include "pinList.h"
#include "ValveDefinitions.h"
#include "PyroDefinitions.h"
#include "AutoSequenceDefinitions.h"
#include "SensorDefinitions.h"

//Trying to figure out RTC stuff with these libs
/* #include <TimeLib.h>
#include <DS1307RTC.h> */

//For use in doing serial inputs as CAN commands for testing
uint8_t fakeCANmsg;

bool abortHaltFlag; //creates halt flag

///// NODE DECLARATION!!!!! /////
//default sets to max nodeID intentionally to be bogus until otherwise set
uint8_t nodeID;       //engine node = 2, prop node = 3, Pasafire node = 8
uint8_t nodeIDfromEEPROM;   //nodeID read out of EEPROM
bool nodeIDdetermine = true;   //boolean flag for if startup is to run the nodeID detect read


///// ADC /////
ADC* adc = new ADC();

elapsedMillis sinceRead1;
elapsedMillis sinceReadRTD;

///// LED /////
elapsedMillis sinceLED;

bool ledstate = 0;

///// CAN /////
CAN_message_t message;
CAN_message_t rxmsg;
CAN_message_t extended;

int value = 0;
int counter = 0;
int MCUtempPIN = 70;  //?? Not sure, I was trying to figure out how to read direct from the non Teensy MCU pin
int MCUtempraw;
int MCUtempCANID = 100; 

int busSpeed0 = 500000; //baudrate
int busSpeed1 = 500000; //baudrate

bool startup{true}; // bool for storing if this is the first loop on startup

uint32_t loopCount {0};// for debugging

// Set the global command, and global state
Command currentCommand{command_NOCOMMAND}; 
State currentState{State::passive};
State priorState;

//AutoSequence stuff for main
int64_t currentCountdownForMain;

// Set EEPROM address for storing states
uint8_t stateAddress{0};
uint8_t nodeIDAddress{1};

///// Temp Sensor for TC Cold Junction /////
//Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
int roundedtemp;


// -------------------------------------------------------------
// abort reset function -- NOT TESTED IN CURRENT VERSION, needs to be worked on
/* void abortReset()
{
  cli();
  EEPROM.update(stateAddress, static_cast<uint8_t>(State::abort)); // write abort code to the EEPROM to be read on restart
  sei();
  digitalWrite(pin::reset, 0);                                       // set reset pin low to restart
} */


// -------------------------------------------------------------
uint8_t NodeIDDetect(uint8_t nodeID, bool startup, bool nodeIDdetermine, uint8_t nodeIDfromEEPROM)       //Function to run the nodeID hardware addressing
{
  if (startup)                          //only on startup assign the pins
    {
    pinMode(pin::NodeAddress0, INPUT);
    pinMode(pin::NodeAddress1, INPUT);
    pinMode(pin::NodeAddress2, INPUT);
    pinMode(pin::NodeAddress3, INPUT);
    }
  
  if (nodeIDdetermine)
    {
    //Read the four digital addressing pins
    uint8_t NodeAddressBit0 = digitalRead(pin::NodeAddress0);
    uint8_t NodeAddressBit1 = digitalRead(pin::NodeAddress1)<<1;
    uint8_t NodeAddressBit2 = digitalRead(pin::NodeAddress2)<<2;
    uint8_t NodeAddressBit3 = digitalRead(pin::NodeAddress3)<<3;
    //Use the read addresses to convert into an int for nodeID
    uint8_t NodeIDAddressRead;
    NodeIDAddressRead = NodeAddressBit0 + NodeAddressBit1 + NodeAddressBit2 + NodeAddressBit3;
    nodeID = NodeIDAddressRead;     //Setting the Global NodeID to detected NodeID
    }
  else
    nodeID = nodeIDfromEEPROM;      //Need to ADD FEATURE where the nodeIDdetermine is variable so on a quick power cycle it doesn't reset, but a manual "shutdown" can
  return nodeID;

  //Need to tweak structure of passing in and returning nodeID
  //Need to implememt any behavior for start up, reset et cetera.
  //Do I need any delays? I did use one in the past but I'd like to eliminate or minimize it
}
// -------------------------------------------------------------


//-------------------------------------------------------//
void setup() {
  startup = true;   // Necessary to set startup to true for the code loop so it does one startup loop for the state machine before entering regular loop behavior
  //nodeID = 3;       //For manually assigning NodeID isntead of the address read, make sure to comment out for operational use

  // -----Read Last State off eeprom and update -----
  currentState = static_cast<State>(EEPROM.read(stateAddress));
  nodeIDfromEEPROM = EEPROM.read(nodeIDAddress);
  startupStateCheck(currentState, currentCommand);

  // ----- Run the Node ID Detection Function -----
  nodeID = NodeIDDetect(nodeID, startup, nodeIDdetermine, nodeIDfromEEPROM);
  //I would like to write this into EEPROM and check on startup with state to make this not able to flip on a power cycle mid test/launch and cutout the setup time

  // ----- Hardware Abort Pin Setup ----- NOT CURRENTLY IN USE
  // This hardware abort allows us to command the Teensy to reboot itself by pulling the reset pin to ground
/*   pinMode(pin::reset, OUTPUT);
  digitalWrite(pin::reset, 1);
  pinMode(pin::abort, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(pin::abort), abortReset, RISING);  */




  // -----Run Valve NodeID Check-----
  ValveNodeIDCheck(valveArray, nodeID);

  // -----Run Valve NodeID Check-----
  ValveEnableNodeIDCheck(valveEnableArray, nodeID);

  // -----Run Valve NodeID Check-----
  PyroNodeIDCheck(pyroArray, nodeID);

  // -----Run Sensor NodeID Check-----
  SensorNodeIDCheck(sensorArray, nodeID);

  // -----Run Valve Setup-----
  valveSetUp(valveArray);

  // -----Run ValveEnable Setup-----
  valveEnableSetUp(valveEnableArray);

  // -----Run Valve Setup-----
  pyroSetUp(pyroArray);

  // -----Run AutoSequence Setup-----
  autoSequenceSetUp(autoSequenceArray);
  
  // -----Run Sensor Setup -----
  sensorSetUp(sensorArray);

  // pin setup
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

///// ADC0 /////
  // reference can be ADC_REFERENCE::REF_3V3, ADC_REFERENCE::REF_1V2 or ADC_REFERENCE::REF_EXT.
  //adc->setReference(ADC_REFERENCE::REF_1V2, ADC_0); // change all 3.3 to 1.2 if you change the reference to 1V2

  adc->adc0->setAveraging(8);                                    // set number of averages
  adc->adc0->setResolution(16);                                   // set bits of resolution
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED_16BITS); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);     // change the sampling speed
  //adc->adc0->recalibrate();

///// ADC1 /////
  adc->adc1->setAveraging(8);                                    // set number of averages
  adc->adc1->setResolution(16);                                   // set bits of resolution
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED_16BITS); // change the conversion speed
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);     // change the sampling speed
  //adc->adc1->recalibrate();

  //delay(500);

///// CAN0 and CAN1 Initialize /////
  Can0.begin(busSpeed0);
  //Can1.begin(busSpeed1); //commented out for Teensy3.5, also not current in use even on 3.6
  pinMode(pin::led, OUTPUT);


///// Temp Sensor for TC Cold Junction /////
// Setting alt I2C pins because I used default I2C pins
Wire.setSDA(38);
Wire.setSCL(37);

/* if(!tempsensor.begin(0x19))
{
  Serial.println("Temp Sensor did not initialize.");
}
else
{
//tempsensor.begin(0x19);
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F
tempsensor.setResolution(2);
  // Match the fastest sample requests in the loop to the sample time for chosen setting or it will not return data
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
} */

  //timer2 = 0;

}

void loop() 
{
  //Display the node number with serial print statement start of each loop
  //Serial.print("NodeID: ");
  //Serial.println(nodeID);


  // --- Read CAN bus and update current command ---
  if(CANread(Can0, currentCommand) && !startup) // do not execute on the first loop
  {
    Serial.print("Command Recieved: ");
    Serial.println(currentCommand);
  }

  while (Serial.available()) 
    {
    fakeCANmsg = Serial.read();
      if(fakeCANmsg  < command_SIZE) //enter 0 inter serial to trigger command read
      {
          //add in code here to prompt for command code and update current command from this
          //Serial.println("Enter Command Byte");
          //CurrentCommand = Serial.read();

              
              //if(fakeCANmsg < command_SIZE)                                           // this checks if the message at that location in the buffer could be a valid command
              //{
                  currentCommand = static_cast<Command>(fakeCANmsg);
              //}

          Serial.println("Command Entered");

        }
    }


  // -----Process Commands Here-----
  //currentCommand = command_vent;    //TESTING COMMAND INPUT ONLY
  commandExecute(currentState, priorState, currentCommand, valveArray, pyroArray, valveEnableArray, autoSequenceArray, sensorArray, abortHaltFlag);


  ////// ABORT FUNCTIONALITY!!!///// This is what overrides main valve and igniter processes! /////
  ////// DO NOT MOVE BEFORE "commandExecute" or after "valveTasks"/"pyroTasks"!!! /////
  haltFlagCheck(abortHaltFlag, valveArray, pyroArray, valveEnableArray);

  // -----Advance needed propulsion system tasks (valve, valve enables, pyro, autosequences) -----
  autoSequenceTasks(autoSequenceArray,nodeID);
  currentCountdownForMain = IgnitionAutoSequence.getCurrentCountdown();
  autoSequenceValveUpdate(valveArray, currentCountdownForMain);
  autoSequencePyroUpdate(pyroArray, currentCountdownForMain);  
  valveTasks(valveArray, nodeID);
  valveEnableTasks(valveEnableArray, nodeID);
  pyroTasks(pyroArray, nodeID);
  sensorTasks(sensorArray, adc, nodeID);
  

/*     Serial.print("abortHaltFlag: ");
    Serial.println(abortHaltFlag); */


  // -----Update State on EEPROM -----
  cli(); // disables interrupts to protect write command
  EEPROM.update(stateAddress, static_cast<uint8_t>(currentState));      // Never use .write()
  EEPROM.update(nodeIDAddress, nodeID);                                 // Never use .write()
  sei(); // reenables interrupts after write is completed

  //CAN State Report and Sensor data send Functions
  CAN2PropSystemStateReport(Can0, currentState, currentCommand, valveArray, pyroArray, valveEnableArray, abortHaltFlag, nodeID);
  CAN2AutosequenceTimerReport(Can0, autoSequenceArray, abortHaltFlag, nodeID);
  SensorArrayCANSend(Can0, sensorArray);

  /* if (sinceRead1 >= 1000000) ///// If Using the old functional sensor reads this WILL BREAK them via resetting the same timer
  {
  //Main Loop state and command print statements - for testing only
  Serial.print("currentState :");
  Serial.println(static_cast<uint8_t>(currentState));
  Serial.print("currentCommand :");
  Serial.println(currentCommand);

  sinceRead1 = 0; //resets timer to zero each time the ADC is read
  //Serial.print("EEPROM Node ID Read :");
  //Serial.println(EEPROM.read(nodeIDAddress));
  } */

startup = false;


}