#include "SensorClass.h"
#include <Arduino.h>
#include <ADC.h>
#include <array>
//#include <ADC_util.h>

using std::string;

// Constructor
SENSOR::SENSOR(int setSensorID, string setSens_Name, SensorType setSensType, int setADCinput, int setSens_Sample_rate, int setBitDepth)
                : sensorID{setSensorID}, sens_name{setSens_Name}, senstype{setSensType}, ADCinput{setADCinput}, sens_sample_rate{setSens_Sample_rate}, bitDepth{setBitDepth}
{
    // place any constructor operations here
}

void SENSOR::begin()
{
    pinMode(ADCinput, INPUT);
}

void SENSOR::resetTimer()
{
    timer = 0;
}

void SENSOR::read(ADC* adc)
{
    currentRawValue = adc->analogRead(ADCinput);
}

