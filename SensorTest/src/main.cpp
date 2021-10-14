#include <Arduino.h>
#include "TempSensorMCP.h"
#include "Thermocouple.h"

TempSensorMCP tempSensor;
Thermocouple thermocoupleOne;
void setup()
{
  const uint8_t tempsensor_pin = 0;
  tempSensor = TempSensorMCP(tempsensor_pin);

  const uint8_t thermocouple_pin1 = 0, thermocouple_pin2 = 0;
  thermocoupleOne = Thermocouple(thermocouple_pin1, thermocouple_pin2, tempSensor);
}

void loop()
{
  float tempC = tempSensor.getTempC();
}