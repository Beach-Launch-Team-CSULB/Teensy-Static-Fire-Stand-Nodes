#include "BitChopper.h"
#include <Arduino.h>

BitChopper::BitChopper()
{

    T_thermocouple = {3, 3};
}

config BitChopper::getT_Thermocouple()
{
    return T_thermocouple;
}
uint32_t BitChopper::compress(config sensorName, uint32_t ADC_Data)
{
    // >> right shift
    // << left shift
    uint32_t mask = (1 << sensorName.resolution) - 1;//build mask with resolution bits
    //Serial.println(mask, BIN);//testing
    ADC_Data = ADC_Data >> sensorName.offset;//shift data right to line it up with mask
    ADC_Data = ADC_Data & mask;//bitwise and to delete all irrelevant data.
    return ADC_Data;
}
uint32_t BitChopper::extract(config sensorName, uint32_t ADC_Data)
{
    // >> right shift
    // << left shift
    ADC_Data = ADC_Data << sensorName.offset;//shift data left to restore to original position
    return ADC_Data;
}

