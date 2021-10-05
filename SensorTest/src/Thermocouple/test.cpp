
#include <./Thermocouple/Thermocouple.h>
#include <Arduino.h>

TypeT typeT;

void setup()
{
    Serial.begin(9600);
}

// In a real use case, this function would get the thermocouple voltage from your
// analog to digital converter and return the value in millivolts.  I'm putting in
// 0.349 mV (yes, 349 uV) as a sample value
float getThermocoupleMillivolts()
{
    return 0.349;
}

// In a real use case, this function would get the cold junction temperature in
// degrees C.  I'm putting in 23.2 degrees as a sample value
float getColdJunctionTemp()
{
    return 23.2;
}

void loop()
{
    // Get the cold junction temperature
    float Tcj = getColdJunctionTemp();

    // Get the thermocouple voltage measurement in millivolts
    float Vtc = getThermocoupleMillivolts();

    Serial.print("\nCold juct temp = ");
    Serial.print(Tcj, 2);
    Serial.print(" deg C\n");

    Serial.print("Thermocouple voltage = ");
    Serial.print(Vtc, 3);
    Serial.print(" mV\n");

    float Ttc = typeT.getTemperature(Vtc, Tcj);

    Serial.print("Thermocouple temp = ");
    Serial.print(Ttc, 2);
    Serial.print(" deg C\n");

    delay(1000);
}
