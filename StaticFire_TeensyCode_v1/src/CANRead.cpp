#include <Arduino.h>
#include <FlexCAN.h>
#include <array>
#include "CANRead.h"
#include "ControlCommands.h"

// function to be run every loop to check for a new CAN message

bool CANread(FlexCAN& CANbus, Command& CurrentCommand)
{
    // New Message Flag
    bool NewMessage {false};

    // create a buffer to hold messages (using a static array for speed)
    static std::array<Command, 64> CommandBuffer{};                                         // large enough for a back up of 8 full frames
    static uint32_t CommandBufferIndex {0};                                                 // keeps track of where we are in command buffer, works just like a stack
    static uint32_t CommandBufferPull {0};                                                  // lets us pull from oldest without shuffling the array

    // create a variable to hold the current message
    CAN_message_t msg {};

    // check if new message in the mailbox
    if(CANbus.available())
    {
        if(CANbus.read(msg))                                                                // read occurs inside if statement
        {
            NewMessage = true;                                                              // set new message flag to true if message recieved and read

            // add CAN messages to internal buffer
            for(uint8_t index{0}; index < msg.len; ++index)                                 // restrict to length of message
            {
                if(msg.buf[index] < command_SIZE)                                           // this checks if the message at that location in the buffer could be a valid command
                {
                    // convert message to a Command type and save it to the buffer
                    CommandBuffer.at(CommandBufferIndex) = static_cast<Command>(msg.buf[index]);
                    ++CommandBufferIndex;                                                   // increment buffer index
                }
            }


        }
    }

    // check if there are commands waiting in the buffer to be executed
    if ((CommandBufferIndex > 0) && (CommandBufferPull != CommandBufferIndex))
    {
        CurrentCommand = CommandBuffer.at(CommandBufferPull);                               // THIS IS WHERE THE WRITE TO CURRENTCOMMAND HAPPENS
        ++CommandBufferPull;      
    }
    else if (CommandBufferPull >= CommandBufferIndex)                                       // check if we've caught up and executed all commands, ie when CommandBufferPull == CommandBufferIndex. CommandBufferPull should NEVER exceed CommandBufferIndex, but for safety I use >=
    {
        CommandBufferPull = 0;
        CommandBufferIndex = 0;
        CurrentCommand = command_NOCOMMAND;                                                 // if we caught up, set command to no command
    }

    // at this point the mailbox has been checked, any waiting messages have been moved to the buffer, and the oldest command has been moved into the global CurrentCommand variable
    // return the message read bool
    return NewMessage;

}