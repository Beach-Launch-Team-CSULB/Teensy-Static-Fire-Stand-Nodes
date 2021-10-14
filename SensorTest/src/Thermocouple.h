#include "Sensor.h"
#include "TempSensorMCP.h"
class Thermocouple : public Sensor
{
public:
    Thermocouple(uint8_t pinOne, uint8_t pinTwo, TempSensorMCP tempSensorJunction);
    float readTemp();

protected:
    int pinOne;
    int pinTwo;
    double getColdJunctionTemp();
    float lookupMV(float temperature);
    float lookupTMP(float millivolts);
};
