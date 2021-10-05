
#ifndef _THERMOCOUPLE_H_
#define _THERMOCOUPLE_H_

#define TC_OUT_OF_RANGE (AbstractThermocouple::TCOUPLE_OUT_OF_RANGE)

class AbstractThermocouple
{
public:
    float getTemperature(float millivolts, float Tcj);
    double getTemperature(double millivolts, double Tcj);
    const double TCOUPLE_OUT_OF_RANGE = -1000.0;

protected:
    double computeColdJunction(double Tcj, const double T0, const double V0, const double p1, const double p2, const double p3, const double p4, const double q1, const double q2);
    double computeTemperatureInternal(double millivolts, const double T0, const double V0, const double p1, const double p2, const double p3, const double p4, const double q1, const double q2, const double q3);
    virtual double computeTemperature(double millivolts) = 0;
    virtual double computeColdJunctionVoltage(double Tcj) = 0;
};

class TypeT : public AbstractThermocouple
{
protected:
    double computeTemperature(double millivolts);
    double computeColdJunctionVoltage(double Tcj);
};

#endif