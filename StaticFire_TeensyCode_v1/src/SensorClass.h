#ifndef SENSORCLASS_H
#define SENSORCLASS_H

// THIS FILE WAS FORMERLY SensorHeader.h

#include <Arduino.h>
#include <string>
#include <bitset>
#include <ADC.h>


using std::string;


// enum for holding sensor types
enum SensorType
{
  pt1000,
  pt5000,
  loadcell1000,
  tcktype,
  rtd,
};

class SENSOR
{
  private:
    const int sensorID;
    const string sens_name;           //your own name for sensor to reference it
    const SensorType senstype;        //sensor type from pt,tc,load_cell, or any others added (update options here as they are added for reference) (Changed this to enum)
    const int ADCinput;               //the input that will be read for this sensor that will get used in the ADC read main loop
    const int sens_sample_rate;       //the sample rate this given sensor will be read at
    elapsedMillis timer;              // timer for future sensor timing operations
    int currentRawValue{};            // holds the current value for the sensor
    const int bitDepth;               // bit depth of the sample

  public:
    SENSOR(int setSensorID, string setSens_Name, SensorType setSensType, int setADCinput, int setSens_Sample_rate, int setBitDepth);

    // Access functions defined in place
    int getSensorID(){return sensorID;}
    string getSensorName(){return sens_name;}
    SensorType getSensorType(){return senstype;}
    int getADCinput(){return ADCinput;}
    int getSensSampleRate(){return sens_sample_rate;}
    int getCurrentRawValue(){return currentRawValue;}
    int getBitDepth(){return bitDepth;}

    // further fuctions defined in SensorClass.cpp
    void begin();                     // run in setup to get pins going
    
    void resetTimer();                // resets timer to zero

    void read(ADC* adc);              // updates currentRawValue with current reading, using an activated ADC object

};

// need to add differential read toggle somehow 
// - differential boolean variable that allows second input to be chosen or defaulted to correct option
// need to add a way to set other SENSOR types like the RTD sensors over I2C (we'd probably want multiple classes. ADCsensors, I2C sensors, SPI sensors etc - Mat)
// - maybe not the right call to roll into this? Hmm. Need to establish use of SENSOR class with sample rates and real read/sends to see what is better
// That will set me up for incorporating the external ADCs later



#endif