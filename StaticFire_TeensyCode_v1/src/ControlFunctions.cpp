#include "ControlFunctions.h"
#include "pinList.h"

void startupStateCheck(const State& currentState, Command& currentCommand)
{
    switch (currentState)
    {
    case State::passive:
        currentCommand = command_passive;
        break;
    case State::test:
        currentCommand = command_test;
        break;
    case State::loxLoad:
        currentCommand = command_loxLoad;
        break;
    case State::pressArm:
        currentCommand = command_pressArm;
        break;
    case State::pressurize:
        currentCommand = command_pressurize;
        break;
    case State::abort:
        currentCommand = command_abort;
        break;
    case State::fireArmed:
        currentCommand = command_fireArm;
        break;
    case State::fire: // if we powercycle mid fire, we just vent
        currentCommand = command_vent;
        break;
    case State::vent:
        currentCommand = command_vent;
        break;
    
    default:
        break;
    }
}

void commandExecute(State& currentState, Command& currentCommand, const std::array<Valve*, NUM_VALVES>& valveArray)
{
    switch (currentCommand)
    {
        case command_debug:
            currentState = State::debug;
            break;
        case command_passive:
            valveArray.at(0)->setState(ValveState::OpenCommanded);
            valveArray.at(1)->setState(ValveState::CloseCommanded);
            valveArray.at(2)->setState(ValveState::CloseCommanded);;
            valveArray.at(3)->setState(ValveState::CloseCommanded);
            valveArray.at(4)->setState(ValveState::CloseCommanded);
            valveArray.at(5)->setState(ValveState::CloseCommanded);
            digitalWrite(pin::isoSafe, 0);
            digitalWrite(pin::fuelVentSafe, 0);
            digitalWrite(pin::loxVentSafe, 0);
            currentState = State::passive;
            break;
        case command_test:
            currentState = State::test;
            break;

// Fire Sequence commands will only be executed from the proper state
        case command_loxLoad:
            if(currentState == State::passive)
            {
                valveArray.at(0)->setState(ValveState::OpenCommanded);
                valveArray.at(1)->setState(ValveState::CloseCommanded);
                valveArray.at(2)->setState(ValveState::CloseCommanded);;
                valveArray.at(3)->setState(ValveState::CloseCommanded);
                valveArray.at(4)->setState(ValveState::CloseCommanded);
                valveArray.at(5)->setState(ValveState::CloseCommanded);
                digitalWrite(pin::isoSafe, 0);
                digitalWrite(pin::fuelVentSafe, 0);
                digitalWrite(pin::loxVentSafe, 0);
                currentState = State::loxLoad;
            }
            break;
        case command_pressArm:
            if(currentState == State::loxLoad)
            {
                valveArray.at(0)->setState(ValveState::CloseCommanded);
                valveArray.at(1)->setState(ValveState::CloseCommanded);
                valveArray.at(2)->setState(ValveState::CloseCommanded);;
                valveArray.at(3)->setState(ValveState::CloseCommanded);
                valveArray.at(4)->setState(ValveState::CloseCommanded);
                valveArray.at(5)->setState(ValveState::CloseCommanded);
                digitalWrite(pin::isoSafe, 1);
                digitalWrite(pin::fuelVentSafe, 0);
                digitalWrite(pin::loxVentSafe, 1);
                currentState = State::pressArm;
            }
            break;
        case command_pressurize:
            if(currentState == State::pressArm)
            {
                valveArray.at(0)->setState(ValveState::CloseCommanded);
                valveArray.at(1)->setState(ValveState::CloseCommanded);
                valveArray.at(2)->setState(ValveState::OpenCommanded);;
                valveArray.at(3)->setState(ValveState::OpenCommanded);
                valveArray.at(4)->setState(ValveState::CloseCommanded);
                valveArray.at(5)->setState(ValveState::CloseCommanded);
                digitalWrite(pin::isoSafe, 1);
                digitalWrite(pin::fuelVentSafe, 0);
                digitalWrite(pin::loxVentSafe, 1);
                currentState = State::pressurize;
            }
            break;
        case command_abort:
                valveArray.at(0)->setState(ValveState::OpenCommanded);
                valveArray.at(1)->setState(ValveState::OpenCommanded);
                valveArray.at(2)->setState(ValveState::OpenCommanded);;
                valveArray.at(3)->setState(ValveState::OpenCommanded);
                valveArray.at(4)->setState(ValveState::CloseCommanded);
                valveArray.at(5)->setState(ValveState::CloseCommanded);
                digitalWrite(pin::isoSafe, 1);
                digitalWrite(pin::fuelVentSafe, 1);
                digitalWrite(pin::loxVentSafe, 0);
                currentState = State::abort;
            break;
        case command_fireArm:
            if(currentState == State::pressurize)
            {
                valveArray.at(0)->setState(ValveState::CloseCommanded);
                valveArray.at(1)->setState(ValveState::CloseCommanded);
                valveArray.at(2)->setState(ValveState::OpenCommanded);;
                valveArray.at(3)->setState(ValveState::OpenCommanded);
                valveArray.at(4)->setState(ValveState::CloseCommanded);
                valveArray.at(5)->setState(ValveState::CloseCommanded);
                digitalWrite(pin::isoSafe, 1);
                digitalWrite(pin::fuelVentSafe, 0);
                digitalWrite(pin::loxVentSafe, 1);
                currentState = State::fireArmed;
            }
            break;
        case command_fire:
            if(currentState == State::fireArmed)
            {
                valveArray.at(0)->setState(ValveState::CloseCommanded);
                valveArray.at(1)->setState(ValveState::CloseCommanded);
                valveArray.at(2)->setState(ValveState::OpenCommanded);;
                valveArray.at(3)->setState(ValveState::OpenCommanded);
                valveArray.at(4)->setState(ValveState::FireCommanded);
                valveArray.at(5)->setState(ValveState::FireCommanded);
                digitalWrite(pin::isoSafe, 1);
                digitalWrite(pin::fuelVentSafe, 0);
                digitalWrite(pin::loxVentSafe, 1);
                currentState = State::fire;
            }
            break;
        case command_vent:
            valveArray.at(0)->setState(ValveState::OpenCommanded);
            valveArray.at(1)->setState(ValveState::OpenCommanded);
            valveArray.at(2)->setState(ValveState::CloseCommanded);;
            valveArray.at(3)->setState(ValveState::CloseCommanded);
            valveArray.at(4)->setState(ValveState::CloseCommanded);
            valveArray.at(5)->setState(ValveState::CloseCommanded);
            digitalWrite(pin::isoSafe, 0);
            digitalWrite(pin::fuelVentSafe, 1);
            digitalWrite(pin::loxVentSafe, 0);
            currentState = State::fire;
            break;
        case command_closeLoxVent:
            if(currentState == State::test)
            {
                valveArray.at(0)->setState(ValveState::CloseCommanded);
            }
            break;
        case command_openLoxVent:
             if(currentState == State::test)
            {
                valveArray.at(0)->setState(ValveState::OpenCommanded);
            }
            break;
        case command_closeLoxIso:
            if(currentState == State::test)
            {
                valveArray.at(2)->setState(ValveState::CloseCommanded);
            }
            break;
        case command_openLoxIso:
             if(currentState == State::test)
            {
                valveArray.at(2)->setState(ValveState::OpenCommanded);
            }
            break; 
        case command_closeFuelVent:
            if(currentState == State::test)
            {
                valveArray.at(1)->setState(ValveState::CloseCommanded);
            }
            break;
        case command_openFuelVent:
             if(currentState == State::test)
            {
                valveArray.at(1)->setState(ValveState::OpenCommanded);
            }
            break;
        case command_closeFuelIso:
            if(currentState == State::test)
            {
                valveArray.at(3)->setState(ValveState::CloseCommanded);
            }
            break;
        case command_openFuelIso:
             if(currentState == State::test)
            {
                valveArray.at(3)->setState(ValveState::OpenCommanded);
            }
            break;
        case command_closeFuelMV:
            if(currentState == State::test)
            {
                valveArray.at(5)->setState(ValveState::CloseCommanded);
            }
            break;
        case command_openFuelMV:
             if(currentState == State::test)
            {
                valveArray.at(5)->setState(ValveState::OpenCommanded);
            }
            break;
        case command_closeLoxMV:
            if(currentState == State::test)
            {
                valveArray.at(4)->setState(ValveState::CloseCommanded);
            }
            break;
        case command_openLoxMV:
             if(currentState == State::test)
            {
                valveArray.at(4)->setState(ValveState::OpenCommanded);
            }
            break;
        case command_disableIsoSafety:
            if(currentState == State::test)
            {
                digitalWrite(pin::isoSafe, 0);
            }
            break;
        case command_enableIsoSafety:
            if(currentState == State::test)
            {
                digitalWrite(pin::isoSafe, 1);
            }
            break;
        case command_disableFuelVentSafety:
            if(currentState == State::test)
            {
                digitalWrite(pin::fuelVentSafe, 0);
            }
            break;
        case command_enableFuelVentSafety:
            if(currentState == State::test)
            {
                digitalWrite(pin::fuelVentSafe, 1);
            }
            break;
        case command_disableLoxVentSafety:
            if(currentState == State::test)
            {
                digitalWrite(pin::loxVentSafe, 0);
            }
            break;
        case command_enableLoxVentSafety:
            if(currentState == State::test)
            {
                digitalWrite(pin::loxVentSafe, 1);
            }
            break;
        default:
            break;
    }
}