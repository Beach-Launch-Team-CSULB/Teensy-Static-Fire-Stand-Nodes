#ifndef CONTROLCOMMANDS_H
#define CONTROLCOMMANDS_H

// this enum hold all commands to be sent to the teensy. All commands assumed to be uint8_t type 

// in main file as global declare: Command CurrentCommand{command_NOCOMMAND} to hold the current command

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




#endif