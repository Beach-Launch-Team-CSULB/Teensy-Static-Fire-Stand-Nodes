#include "TempSensor.h"
#include "Adafruit_MCP9808.h"
#include "Wire.h"

// Create the MCP9808 temperature sensor object

class TempSensorMCP : public TempSensor
{
public:
    TempSensorMCP(uint8_t pinOut);
    TempSensorMCP();

    float getTempC();

    float getTempF();

    //int processMiniPacket(MiniPacket packet);

    void shutdown();
};

// void setup()
// {
//     Serial.begin(9600);
//     while (!Serial)
//         ; //waits for serial terminal to be open, necessary in newer arduino boards.
//     Serial.println("MCP9808 demo");

//     if (!tempsensor.begin(0x18))
//     {
//         Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
//         while (1)
//             ;
//     }

//     tempsensor.setResolution(3);
// }

// void loop()
// {
//     Serial.println("wake up MCP9808.... "); // wake up MCP9808 - power consumption ~200 mikro Ampere
//     tempsensor.wake();                      // wake up, ready to read!

//     // Read and print out the temperature, also shows the resolution mode used for reading.
//     Serial.print("Resolution in mode: ");
//     Serial.println(tempsensor.getResolution());
//     float c = tempsensor.readTempC();
//     float f = tempsensor.readTempF();
//     Serial.print("Temp: ");
//     Serial.print(c, 4);
//     Serial.print("*C\t and ");
//     Serial.print(f, 4);
//     Serial.println("*F.");

//     delay(2000);
//     Serial.println("Shutdown MCP9808.... ");
//     tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
//     Serial.println("");
//     delay(200);
// }