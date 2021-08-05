#include <string>
#include <bitset>

//#include <iostream>

using std::string;

// senstype options, add and expand as sensor types come up
    //Add in the sensetype CAN ID formatting into the class declaration, or maybe split to whole CAN ID formatting header file?
// pt1000
// pt5000
// load_cell1000
// tcktype
// rtd
// 

class SENSOR
{
  public:
    string sens_name{};           //your own name for sensor to reference it
    string senstype{};            //sensor type from pt,tc,load_cell, or any others added (update options here as they are added for reference)
    int ADCinput{};               //the input that will be read for this sensor that will get used in the ADC read main loop
    int sens_sample_rate{};       //the sample rate this given sensor will be read at
};

// need to add differential read toggle somehow 
// - differential boolean variable that allows second input to be chosen or defaulted to correct option
// need to add a way to set other SENSOR types like the RTD sensors over I2C
// - maybe not the right call to roll into this? Hmm. Need to establish use of SENSOR class with sample rates and real read/sends to see what is better
// That will set me up for incorporating the external ADCs later


///// DANS ROCKET CAN FORMAT /////
int main ()
{

string FrameType{};     // Control, SensorData, StateReport

std::bitset<11> ID_A;
std::bitset<18> ID_B;



// Node IDs //

//Test Outputs//
//std::cout << "ID A: " << ID_A << '\n';
//std::cout << "ID B: " << ID_B << '\n';
}
