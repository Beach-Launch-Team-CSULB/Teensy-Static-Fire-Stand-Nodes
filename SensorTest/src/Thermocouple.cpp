#include "Thermocouple.h"
typedef unsigned char uint8_t;

class Thermocouple
{
public:
    Thermocouple(uint8_t pinOne, uint8_t pinTwo, TempSensorMCP tempSensorJunction)
    {
        tempSensor = tempSensorJunction;
        pinOne = pinOne;
        pinTwo = pinTwo;
    }
    float readTemp()
    {
        float coldJunctionTemp = getColdJunctionTemp();
        float coldJunctionMV = lookupMV(coldJunctionTemp);
        float thermocoupleMV = readPins();
        float totalMV = (coldJunctionMV + thermocoupleMV);
        return lookupTMP(totalMV);
    }

protected:
    TempSensorMCP tempSensor;
    uint8_t pinOne, pinTwo;
    float getColdJunctionTemp()
    {
        return tempSensor.getTempC();
    };
    float lookupMV(float temperature)
    {
        //lookupTable
        return 5;
    }
    float lookupTMP(float millivolts)
    {
        //lookupTable
        return 5;
    }
    float readPins()
    {
        return 3;
        //analogRead(pinOne)...
    }
};
