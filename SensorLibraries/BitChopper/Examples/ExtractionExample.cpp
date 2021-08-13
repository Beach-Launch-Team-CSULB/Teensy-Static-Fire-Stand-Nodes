#include <Arduino.h>
#include "BitChopper.h"
void setup()
{
    // no setup required
}
void loop()
{
    BitChopper bitChopper;      //use default constructor
    uint32_t data = 3723476863; //110 1110 11110 111110 1111110 1111111

    config testConfig;
    testConfig.resolution = 16;
    testConfig.offset = 0; // how many bits over from the zero index relavent bits are

    Serial.println(data, BIN);
    data = bitChopper.compress(testConfig, data);
    data = bitChopper.extract(testConfig, data);
    Serial.println(data, BIN);

    delay(3000);
}