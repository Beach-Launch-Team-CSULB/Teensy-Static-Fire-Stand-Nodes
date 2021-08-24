#include "ValveClass.h"
#include "ValveStates.h"
#include <Arduino.h>



Valve::Valve(int setValveID, ValveType setValveType, int setPin, uint32_t setFullDutyTime, uint32_t setFireDelay=0, uint8_t setHoldDuty=50)
                : valveID{setValveID}, valveType{setValveType}, pin{setPin}, fullDutyTime{setFullDutyTime}, fireDelay{setFireDelay}, holdDuty{setHoldDuty}
{
    switch (valveType)
    {
        case NormalClosed:
            state = ValveState::Closed;
            break;
        case NormalOpen:
            state = ValveState::Open;
            break;
        default:
            state = ValveState::Closed;
            break;
    }
    timer = 0;
    //pinMode(pin, OUTPUT); had to move this to begin so that pins would be set from within setup()
}

void Valve::begin()
{
    pinMode(pin, OUTPUT);
    analogWrite(pin, 0);
}

void Valve::resetTimer()
{
    timer = 0;
}

void Valve::stateOperations()
{


    switch (state)
    {
    // if a valve has been commanded to fire, it will start actuation after appropriate delay, normal closed actuate open, normal open actuate closed
    // every state change should reset the timer
    case ValveState::FireCommanded:
        if(timer >= fireDelay)
        {
            switch (valveType)
            {
                case NormalClosed:
                    state = ValveState::OpenCommanded;
                    timer = 0;
                    break;
                case NormalOpen:
                    state = ValveState::CloseCommanded;
                    timer = 0;
                    break;
                default:
                    break;
            }
        }
        break;

    // if a valve is commanded open, if its normal closed it needs to fully actuate, if normal open it needs to drop power to zero
    case ValveState::OpenCommanded:
        switch (valveType)
        {
            case NormalClosed:
                analogWrite(pin, fullDuty);
                timer = 0;
                state = ValveState::OpenProcess;
                break;
            case NormalOpen:
                analogWrite(pin, 0);
                timer = 0;
                state = ValveState::Open;
                break;
            default:
                break;
        }
        break;

    // if a valve is commanded closed, a normal closed removes power, normal open starts activation sequence
    case ValveState::CloseCommanded:
        switch (valveType)
        {
            case NormalClosed:
                analogWrite(pin, 0);
                timer = 0;
                state = ValveState::Closed;
                break;
            case NormalOpen:
                analogWrite(pin, fullDuty);
                timer = 0;
                state = ValveState::CloseProcess;
                break;
        }
        break;

    // if a valve is in OpenProcess, check if the fullDutyTime has passed. If it has, cycle down to hold duty
    case ValveState::OpenProcess:
        if(timer >= fullDutyTime)
        {
            analogWrite(pin, holdDuty);
            timer = 0;
            state = ValveState::Open;
        }
        break;

    // if a valve is in CloseProcess, check if the fullDutyTime has passed. If it has, cycle down to hold duty
    case ValveState::CloseProcess:
        if(timer >= fullDutyTime)
        {
            analogWrite(pin, holdDuty);
            timer = 0;
            state = ValveState::Closed;
        }
        break;
    case ValveState::Closed:
        switch (valveType)
        {
            case NormalClosed:
                analogWrite(pin, 0);
                break;
            default:
                break;
        }
    
    // All other states require no action
    default:
        break;
    }
}
