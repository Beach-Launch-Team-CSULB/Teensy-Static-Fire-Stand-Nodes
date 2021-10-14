class Sensor
{
public:
    //virtual int processMiniPacket(MiniPacket packet) = 0; // Pure virtual function is declared as follows.

    MiniPacket readData();

protected:
    MiniPacket latest;
};
//read()
//
//interpret(MiniPacket)