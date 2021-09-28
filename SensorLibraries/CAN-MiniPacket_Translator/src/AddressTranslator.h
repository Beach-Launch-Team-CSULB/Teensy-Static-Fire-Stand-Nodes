#include <Arduino.h>

#ifndef AddressTranslator_H
#define AddressTranslator_H


/*IMPORTANT! YOU MUST READ THIS

If any of these enumerations are ever written to file, it is CRITICAL
that they do not change. Any MiniPackets(sensor readings) written 
directly to file will count later on using these addresses to decode
the MiniPacket binary.

If possible, I recommend writing a string identifier to file. IE
write "LoxTankUpperTC" instead of 1 to the persistent storage.
This human readable format will be resistent to human error. 
Additionally, we could record a timestamp for each data sample,
so we can revert to a version of the address table required to 
decode the samples at that time. 

Remember, changing these values only has a cost after real data 
has been recorded. Before that point, it doesn't matter. 
*/

//Used as: messagePriority
enum frame
{
    commandFrame,
    dataFrame,
    stateReportFrame
};

//Used as: nodeID
enum node
{
    rocketNode,
    guiNode
};

//Used as: MiniPacket ID
enum RocketNodeSensor
{
    ReservedAddress = 0, //Zero is AbstractCanPacket null address
    LoxTankUpperTC,
    LoxTankMidTC,
    LoxTankLowerTC,
    ChamberExternalTC,
    ChamberPT1,
    ChamberPT1Backup,
    LRPT2,
    LRPT3,
    ThrustMountLoadCell1,
    ThrustMountLoadCell2,
    ThrustMountLoadCell3,
    ColdJunctionTemp1,
    ColdJunctionTemp2,
    DomeRegFuelPT,
    DomeRegLOXPT,
    FuelTankPT,
    LOXTankPT,
    HiPressPT
};

//Used as: MiniPacket ID
enum Command
{
    // we reserve 0 to be a no command state
    command_NOCOMMAND = 0,
    command_debug = 1,
    command_passive = 3,
    command_test = 5,
    command_loxLoad = 7,
    command_pressArm = 9,
    command_pressurize = 11,
    command_abort = 13,
    command_fireArm = 15,
    command_fire = 17,
    command_vent = 19,
    command_closeLoxVent = 30,
    command_openLoxVent = 31,
    command_closeLoxIso = 32,
    command_openLoxIso = 33,
    command_closeFuelVent = 34,
    command_openFuelVent = 35,
    command_closeFuelIso = 36,
    command_openFuelIso = 37,
    command_closeFuelMV = 38,
    command_openFuelMV = 39,
    command_closeLoxMV = 40,
    command_openLoxMV = 41,
    command_disableIsoSafety = 42,
    command_enableIsoSafety = 43,
    command_disableFuelVentSafety = 44,
    command_enableFuelVentSafety = 45,
    command_disableLoxVentSafety = 46,
    command_enableLoxVentSafety = 47, 
    command_SIZE, // not a valid command but it is useful for checking if recieved messages are valid commands, see CANRead. Always leave this at the end of the enum listcomm
};

class AddressTranslator//do I need this?
{
};

#endif
