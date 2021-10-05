#include "Thermocouple.h"

float AbstractThermocouple::getTemperature(float millivolts, float Tcj)
{
    return (float)this->getTemperature((double)millivolts, (double)Tcj);
}

double AbstractThermocouple::getTemperature(double millivolts, double Tcj)
{
    double Vcj = this->computeColdJunctionVoltage(Tcj);
    return this->computeTemperature(millivolts + Vcj);
}

double AbstractThermocouple::computeColdJunction(
    double Tcj,
    const double T0,
    const double V0,
    const double p1,
    const double p2,
    const double p3,
    const double p4,
    const double q1,
    const double q2)
{

#define numerator ((Tcj - T0) * (p1 + (Tcj - T0) * (p2 + (Tcj - T0) * (p3 + p4 * (Tcj - T0)))))
#define denominator (1.0 + (Tcj - T0) * (q1 + q2 * (Tcj - T0)))
    return (V0 + (numerator / denominator));
#undef numerator
#undef denominator
}

double TypeT::computeColdJunctionVoltage(double Tcj)
{
    // Type T equations
    // Tcj = temperature in Celsius
    // Returns equivalent voltage in mV
    const double T0 = 2.5000000E+01;
    const double V0 = 9.9198279E-01;
    const double p1 = 4.0716564E-02;
    const double p2 = 7.1170297E-04;
    const double p3 = 6.8782631E-07;
    const double p4 = 4.3295061E-11;
    const double q1 = 1.6458102E-02;
    const double q2 = 0.0;

    return this->computeColdJunction(Tcj, T0, V0, p1, p2, p3, p4, q1, q2);
}

double TypeT::computeTemperature(double millivolts)
{
    // Type T equations
    // millivolts = voltage in mV
    // Returns computed temperature in Celsius
    double T0, V0, p1, p2, p3, p4, q1, q2, q3;
    if (-6.18 <= millivolts && millivolts < -4.648)
    {
        T0 = -1.9243000E+02;
        V0 = -5.4798963E+00;
        p1 = 5.9572141E+01;
        p2 = 1.9675733E+00;
        p3 = -7.8176011E+01;
        p4 = -1.0963280E+01;
        q1 = 2.7498092E-01;
        q2 = -1.3768944E+00;
        q3 = -4.5209805E-01;
    }
    else if (-4.648 <= millivolts && millivolts < 0.0)
    {
        T0 = -6.0000000E+01;
        V0 = -2.1528350E+00;
        p1 = 3.0449332E+01;
        p2 = -1.2946560E+00;
        p3 = -3.0500735E+00;
        p4 = -1.9226856E-01;
        q1 = 6.9877863E-03;
        q2 = -1.0596207E-01;
        q3 = -1.0774995E-02;
    }
    else if (0.0 <= millivolts && millivolts < 9.288)
    {
        T0 = 1.3500000E+02;
        V0 = 5.9588600E+00;
        p1 = 2.0325591E+01;
        p2 = 3.3013079E+00;
        p3 = 1.2638462E-01;
        p4 = -8.2883695E-04;
        q1 = 1.7595577E-01;
        q2 = 7.9740521E-03;
        q3 = 0.0;
    }
    else if (9.288 <= millivolts && millivolts < 20.872)
    {
        T0 = 3.0000000E+02;
        V0 = 1.4861780E+01;
        p1 = 1.7214707E+01;
        p2 = -9.3862713E-01;
        p3 = -7.3509066E-02;
        p4 = 2.9576140E-04;
        q1 = -4.8095795E-02;
        q2 = -4.7352054E-03;
        q3 = -1.2146518E-02;
    }
    else
    {
        return (AbstractThermocouple::TCOUPLE_OUT_OF_RANGE);
    }

    return this->computeTemperatureInternal(millivolts, T0, V0, p1, p2, p3, p4, q1, q2, q3);
}
