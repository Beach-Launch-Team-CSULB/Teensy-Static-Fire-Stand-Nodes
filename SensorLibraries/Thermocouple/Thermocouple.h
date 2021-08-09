//Author Jacob Waters

/*
 * This class is designed to convert ADC reads from the thermocouple leads
    into a net voltage, and then use a function or lookup table
    to convert the voltage differential into a temperature differential.
    From there, that temperature differential needs to be added to the
    temperature at the measurement junction.
 *
 */
#ifndef Arduino_h
#include "Arduino.h"
#endif

#ifndef Thermocouple_h
#define Thermocouple_h


class Thermocouple
{
	public:
    Thermocouple();
    double voltageToTempMapping(double voltageDifferential, double temperatureReference);
	private:


};

#endif
