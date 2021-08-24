#ifndef CONTROLCOMMANDS_H
#define CONTROLCOMMANDS_H

// this enum hold all commands to be sent to the teensy. All commands assumed to be uint8_t type 

// in main file as global declare: Command CurrentCommand{command_NOCOMMAND} to hold the current command

enum Command
{
    // we reserve 0 to be a no command state
    command_NOCOMMAND, // 0
    command_valveOpen, // 1
    command_valveClosed, // 2
    command_3, // 3
    command_4, // 4
    command_SIZE, // not a valid command but it is useful for checking if recieved messages are valid commands, see CANRead. Always leave this at the end of the enum list
};




#endif