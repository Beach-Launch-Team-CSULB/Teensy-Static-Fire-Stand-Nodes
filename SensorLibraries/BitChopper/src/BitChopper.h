#include "Arduino.h"

#ifndef BitChopper_h
#define BitChopper_h

typedef struct config
{
    uint8_t resolution; //resolution is how many bits wide the sample is
    uint8_t offset;     // how many bits over from the zero index relavent bits are
} config;

//static const config T_thermocouple = {16, 7};//Example values for TESTING
//static const config otherSensor = {8, 12};//Example values for TESTING

class BitChopper
{

public:
    BitChopper();
    uint32_t compress(config sensorName, uint32_t ADC_Data);
    uint32_t extract(config sensorName, uint32_t compressedData);
    config getT_Thermocouple();
private:
    config T_thermocouple;
    config otherSensor;
};

#endif
