#include "SensorClass.h"
#include <Arduino.h>
#include <ADC.h>
#include <array>
//#include <ADC_util.h>

using std::string;

SENSOR::SENSOR(string setSens_Name, SensorType setSensType, int setADCinput, int setSens_Sample_rate)
                : sens_name{setSens_Name}, senstype{setSensType}, ADCinput{setADCinput}, sens_sample_rate{setSens_Sample_rate}
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

