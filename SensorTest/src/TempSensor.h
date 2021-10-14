#include "Sensor.h"
class TempSensor : public Sensor
{
public:
    float getTempC();
    float getTempF();
};