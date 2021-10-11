/*
#include <SensorHeader.h>
#include <Arduino.h>
#include <ADC.h>
#include <ADC_util.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <WireKinetis.h>
#include <Adafruit_MCP9808.cpp>
#include <string>
#include <list>
using std::string;

// should I use the ifdef stuff like I see in libraries for which node this is set to, 
//and write the whole code to each teensy with just a switch in header file for which role it's playing?
//Hmm not sure, might be better to manually split it up. We'll see how header file setup and the proper loop/read implementation goes.


static int node;

///// ADC /////
ADC* adc = new ADC();
#define PINS 23 //was default 25 but skipping two pins for I2C bus use?
#define PINS_DIFF 2
uint8_t adc_pins[] = {A0, A1, A2, A3, A6, A7, A8, A9, A10,
                      A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24};
uint8_t adc_pins_diff[] = {A10, A11};

bool input_enable[25] = {false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};


///// CAN /////
int led = 13;
//FlexCAN CAN0;
//FlexCAN CAN0 (1000000, 0);
//FlexCAN CAN1;
CAN_message_t message;
CAN_message_t rxmsg;
//CAN_message_t extended;
//static uint8_t hex[17] = "0123456789abcdef";
//static CAN_message_t msg;

int value = 0;
int pin = 0;
int counter = 0;

///// SENSORS /////
SENSOR pt1 {"DomeRegFuel", "pt1000", 21, 10};
SENSOR pt2 {"DomeRegLOX", "pt1000", 22, 10};
SENSOR pt3 {"FuelTank", "pt1000", 23, 100};
SENSOR pt4 {"LOXTank", "pt1000", 24, 100};
SENSOR pt5 {"HiPress", "pt5000", 20, 100};
SENSOR pt6 {"Chamber", "pt1000", 0, 1000};
SENSOR load_cell1 {"Thrust Load Cell 1", "loadcell1000", 1, 100};
SENSOR load_cell2 {"Thrust Load Cell 2", "loadcell1000", 2, 100};
SENSOR load_cell3 {"Thrust Load Cell 3", "loadcell1000", 3, 100};
SENSOR tc1 {"External Chamber Wall TC", "tcktype",10,25};
SENSOR tc2 {"LOX Tank Lower TC", "tcktype",12,1};
//SENSOR tc3 {"LOX Tank Middle TC", "tcktype",10,1};
//SENSOR tc4 {"LOX Tank Upper TC", "tcktype",12,1};


///// SV Control /////
int SVenable_1_pin = 24;
int SVenable_2_pin = 25;

///// Temp Sensor for TC Cold Junction /////
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();



//-------------------------------------------------------//
void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < PINS; i++)
    {
        pinMode(adc_pins[i], INPUT);
    }

  Serial.begin(9600);

///// ADC0 /////
  // reference can be ADC_REFERENCE::REF_3V3, ADC_REFERENCE::REF_1V2 or ADC_REFERENCE::REF_EXT.
  //adc->setReference(ADC_REFERENCE::REF_1V2, ADC_0); // change all 3.3 to 1.2 if you change the reference to 1V2

  adc->adc0->setAveraging(4);                                    // set number of averages
  adc->adc0->setResolution(16);                                   // set bits of resolution
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED_16BITS); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);     // change the sampling speed
  //adc->adc0->recalibrate();

///// ADC1 /////
  adc->adc1->setAveraging(4);                                    // set number of averages
  adc->adc1->setResolution(16);                                   // set bits of resolution
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED_16BITS); // change the conversion speed
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);     // change the sampling speed
  //adc->adc1->recalibrate();

  delay(500);

///// CAN0 and CAN1 Initialize /////
  Can0.begin(500000);
  Can1.begin(1000000);
  pinMode(led, OUTPUT);

///// SV Control /////

pinMode(SVenable_1_pin, LOW); //LOW drives it high to enable driver
pinMode(SVenable_2_pin, LOW); //LOW drives it high to enable driver
// pinMode(SVenable_1_pin, HIGH); //HIGH drives it low to disable driver
// pinMode(SVenable_2_pin, HIGH); //HIGH drives it low to disable driver

///// Temp Sensor for TC Cold Junction /////
tempsensor.begin(0x19);
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F
tempsensor.setResolution(3);
  // Match the fastest sample requests in the loop to the sample time for chosen setting or it will not return data
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms

}

void loop() 
{

  for (int i = 0; i < PINS; i++)
    if (input_enable[i] == true) {
      {value = adc->analogRead(adc_pins[i]);
          //adc->adc0->stopPDB();
          //adc->adc1->stopPDB();
          //adc->adc0->enableInterrupts(adc0_isr);
          //adc->adc1->enableInterrupts(adc1_isr);          
          //adc->adc0->startSingleRead(i);
          //adc->adc1->startSingleRead(i);
          //adc->adc0->startPDB(10);
          //adc->adc1->startPDB(10);

          //adc->adc1->startSingleRead(i); // call this to setup everything before the pdb starts

          //adc->adc1->startPDB(100); //frequency in Hz
          Serial.print(" A:");
          Serial.print(i);
          //Serial.print(pt1.sens_name.c_str());
          Serial.println();
          Serial.print(" Raw:");
          Serial.print(value);
          Serial.print(" Converted:");
          Serial.print(value * 3.3 / adc->adc0->getMaxValue(), 2);
          Serial.println();
          counter = counter+1;
          //Serial.print("counter: ");
          //Serial.print(counter);
          Serial.println();
          
          //message.buf[0] = (value >> 8) & 0xff;     //for 24 bit samples from future external ADCs
          message.buf[0] = (value >> 8) & 0xff;
          message.buf[1] = value & 0xff;
          
          message.len = 2;
          //message.len = 3;                          //for 24 bit samples from future external ADCs
          message.id = 0x242;
          Can0.write(message);
          Can1.write(message);
          message.buf[1]++;

          digitalWrite(led, 1);
          delay(50);
          digitalWrite(led, 0);
          delay(50);
      }  
    }


// SV duty cycle and driver chip testing
// analogWrite(2, 256);
// analogWrite(6, 256);
// delay(150);
// analogWrite(2, 51);
// analogWrite(6, 51);
// delay(2500);
// analogWrite(2, 0);
// analogWrite(6, 0);

///// Temp Sensor for TC Cold Junction /////
tempsensor.shutdown_wake(0);
float f = tempsensor.readTempF();
Serial.print("Cold Junction Temp (F)");
Serial.print(f, 4);
delay(250);
tempsensor.shutdown_wake(1);

}
*/