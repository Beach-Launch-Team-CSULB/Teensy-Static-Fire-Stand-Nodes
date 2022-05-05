#include "ControlFunctions.h"


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
    case State::HiPressArm:
        currentCommand = command_HiPressArm;
        break;
    case State::HiPressPressurized:
        currentCommand = command_HiPressPressurized;
        break;
    case State::TankPressArm:
        currentCommand = command_TankPressArm;
        break;
    case State::TankPressPressurized:
        currentCommand = commend_TankPressPressurized;
        break;
    case State::fireArmed:
        currentCommand = command_fireArm;
        break;
    case State::fire: // if we powercycle mid fire, we just vent (maybe shouldn't always be true with multinode systems)
        currentCommand = command_abort;
        break;
    case State::abort:
        currentCommand = command_abort;
        break;
    case State::vent:
        currentCommand = command_vent;
        break;
    default:
        break;
    }
}

void haltFlagCheck(bool & haltFlag, const std::array<Valve*, NUM_VALVES>& valveArray, const std::array<Pyro*, NUM_PYROS>& pyroArray, const std::array<ValveEnable*, NUM_VALVEENABLE>& valveEnableArray)
{
    if(haltFlag)
    {
        valveArray.at(8)->setState(ValveState::CloseCommanded);     // Renegade SF Stand MV
        valveArray.at(9)->setState(ValveState::CloseCommanded);     // Renegade SF Stand MV
        valveArray.at(14)->setState(ValveState::CloseCommanded);    // Pasafire MV
        valveArray.at(15)->setState(ValveState::CloseCommanded);    // Pasafire MV
        pyroArray.at(0)->setState(PyroState::OffCommanded);         // Renegade SF Igniter1
        pyroArray.at(1)->setState(PyroState::OffCommanded);         // Renegade SF Igniter2
        pyroArray.at(2)->setState(PyroState::OffCommanded);         // Pasafire Igniter1
        pyroArray.at(3)->setState(PyroState::OffCommanded);         // Pasafire Igniter2
        valveEnableArray.at(1)->setState(ValveEnableState::Off);    // Renegade SF Stand MV Enable
        valveEnableArray.at(8)->setState(ValveEnableState::Off);    // Pasafire MV Enable
    }
    

}


void commandExecute(State& currentState, State& priorState, Command& currentCommand, const std::array<Valve*, NUM_VALVES>& valveArray, const std::array<Pyro*, NUM_PYROS>& pyroArray, const std::array<ValveEnable*, NUM_VALVEENABLE>& valveEnableArray, const std::array<AutoSequence*, NUM_AUTOSEQUENCES>& autoSequenceArray, const std::array<MCU_SENSOR*, NUM_SENSORS>& sensorArray, bool & haltFlag)
{
    switch (currentCommand)
    {
        case command_debug:
            currentState = State::debug;
            break;
        case command_passive:
            valveArray.at(0)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(10)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::CloseCommanded);        // Pasafire
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2
            valveEnableArray.at(0)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(1)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(2)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(3)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(4)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(5)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);  // Reengade, Global? Not sure how I want to handle multinode autosequence yet
            autoSequenceArray.at(1)->setState(AutoSequenceState::Standby);  // Pasafire
            sensorArray.at(0)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1pos
            sensorArray.at(1)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1neg
            sensorArray.at(2)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2pos
            sensorArray.at(3)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2neg
            sensorArray.at(4)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3pos
            sensorArray.at(5)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3neg
            sensorArray.at(6)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT2
            sensorArray.at(7)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT1
            sensorArray.at(8)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInletPropSidePT
            sensorArray.at(9)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInjectorPT
            sensorArray.at(10)->setState(SensorState::Medium);        // Renegade SF Stand - LoxInletPropSidePT
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand - MVPneumaticsPT
            sensorArray.at(12)->setState(SensorState::Medium);        // Renegade SF Stand - DomeRegFuelPT
            sensorArray.at(13)->setState(SensorState::Medium);        // Renegade SF Stand - DomeRegLoxPT
            sensorArray.at(14)->setState(SensorState::Medium);        // Renegade SF Stand - FuelTankPT
            sensorArray.at(15)->setState(SensorState::Medium);        // Renegade SF Stand - LoxTankPT
            sensorArray.at(16)->setState(SensorState::Medium);        // Renegade SF Stand - HiPressFuelPT
            sensorArray.at(17)->setState(SensorState::Medium);        // Renegade SF Stand - HiPressLoxPT
            sensorArray.at(18)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode2
            sensorArray.at(19)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode3
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireChamberPT1
            sensorArray.at(21)->setState(SensorState::Medium);        // Pasafire - PasafireFuelTankPT
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireLOXTankPT
            sensorArray.at(23)->setState(SensorState::Medium);        // Pasafire - PasafireFuelRegPT
            sensorArray.at(24)->setState(SensorState::Medium);        // Pasafire - PasafireLOXRegPT
            sensorArray.at(25)->setState(SensorState::Medium);        // Pasafire - PasafireFuelLinePT
            sensorArray.at(26)->setState(SensorState::Medium);        // Pasafire - PasafireLOXLinePT
            sensorArray.at(27)->setState(SensorState::Medium);        // Pasafire - PasafirePneumaticPT
            sensorArray.at(28)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1pos
            sensorArray.at(29)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1neg
            sensorArray.at(30)->setState(SensorState::Medium);        // Pasafire - MCUtempNode8
            currentState = State::passive;
            haltFlag = false;
            break;
        case command_test:
            if(currentState == State::passive)
            {
            currentState = State::test;
            valveEnableArray.at(0)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(1)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(2)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(3)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(4)->setState(ValveEnableState::On);        // Renegade SF Stand
            //valveEnableArray.at(5)->setState(ValveEnableState::On);        // Renegade SF Stand                        
            valveEnableArray.at(6)->setState(ValveEnableState::On);        // Pasafire
            //valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::On);        // Pasafire
            }
            break;
        case command_EnterOffNominal:
            priorState = currentState; //for remembering the state the system was in when entering Off Nominal
            currentState = State::offNominal;
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::On);
            valveEnableArray.at(2)->setState(ValveEnableState::On);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            //valveEnableArray.at(5)->setState(ValveEnableState::On);  
            valveEnableArray.at(6)->setState(ValveEnableState::On);        // Pasafire
            //valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::On);        // Pasafire
            break;            
        case command_ExitOffNominal:
            if(currentState == State::offNominal)
            {
            currentState = priorState;              //IS THIS STILL TRUE???? - Beware, this currently doesn't function fully as desired. Will leave ValveEnables all on and not actually enter the prior command
            }
            break;
        case command_abort:
            haltFlag = true;
            currentState = State::abort;
            autoSequenceArray.at(0)->setState(AutoSequenceState::Hold);     // Renegade SF Stand
            autoSequenceArray.at(1)->setState(AutoSequenceState::Hold);     // Pasafire
            break;
        case command_vent:
            valveArray.at(0)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(10)->setState(ValveState::OpenCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::OpenCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::CloseCommanded);        // Pasafire
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::On);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::Off);
            valveEnableArray.at(6)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire
            autoSequenceArray.at(0)->setState(AutoSequenceState::Hold);  // Renegade
            autoSequenceArray.at(1)->setState(AutoSequenceState::Hold);  // Pasafire
            sensorArray.at(0)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1pos
            sensorArray.at(1)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1neg
            sensorArray.at(2)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2pos
            sensorArray.at(3)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2neg
            sensorArray.at(4)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3pos
            sensorArray.at(5)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3neg
            sensorArray.at(6)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT2
            sensorArray.at(7)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT1
            sensorArray.at(8)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInletPropSidePT
            sensorArray.at(9)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInjectorPT
            sensorArray.at(10)->setState(SensorState::Medium);        // Renegade SF Stand - LoxInletPropSidePT
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand - MVPneumaticsPT
            sensorArray.at(12)->setState(SensorState::Medium);        // Renegade SF Stand - DomeRegFuelPT
            sensorArray.at(13)->setState(SensorState::Medium);        // Renegade SF Stand - DomeRegLoxPT
            sensorArray.at(14)->setState(SensorState::Fast);        // Renegade SF Stand - FuelTankPT
            sensorArray.at(15)->setState(SensorState::Fast);        // Renegade SF Stand - LoxTankPT
            sensorArray.at(16)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressFuelPT
            sensorArray.at(17)->setState(SensorState::Medium);        // Renegade SF Stand - HiPressLoxPT
            sensorArray.at(18)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode2
            sensorArray.at(19)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode3
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireChamberPT1
            sensorArray.at(21)->setState(SensorState::Medium);        // Pasafire - PasafireFuelTankPT
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireLOXTankPT
            sensorArray.at(23)->setState(SensorState::Medium);        // Pasafire - PasafireFuelRegPT
            sensorArray.at(24)->setState(SensorState::Medium);        // Pasafire - PasafireLOXRegPT
            sensorArray.at(25)->setState(SensorState::Medium);        // Pasafire - PasafireFuelLinePT
            sensorArray.at(26)->setState(SensorState::Medium);        // Pasafire - PasafireLOXLinePT
            sensorArray.at(27)->setState(SensorState::Medium);        // Pasafire - PasafirePneumaticPT
            sensorArray.at(28)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1pos
            sensorArray.at(29)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1neg
            sensorArray.at(30)->setState(SensorState::Medium);        // Pasafire - MCUtempNode8
            currentState = State::vent;
            break;
// Fire Sequence commands will only be executed from the proper state
        case command_HiPressArm:
            if(currentState == State::passive)
            {
            valveArray.at(0)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(10)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::CloseCommanded);        // Pasafire
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::Off);
            valveEnableArray.at(4)->setState(ValveEnableState::Off);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
            valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);  // Renegade SF Stand
            autoSequenceArray.at(1)->setState(AutoSequenceState::Standby);  // Pasafire
            sensorArray.at(0)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1pos
            sensorArray.at(1)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1neg
            sensorArray.at(2)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2pos
            sensorArray.at(3)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2neg
            sensorArray.at(4)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3pos
            sensorArray.at(5)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3neg
            sensorArray.at(6)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT2
            sensorArray.at(7)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT1
            sensorArray.at(8)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInletPropSidePT
            sensorArray.at(9)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInjectorPT
            sensorArray.at(10)->setState(SensorState::Medium);        // Renegade SF Stand - LoxInletPropSidePT
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand - MVPneumaticsPT
            sensorArray.at(12)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegFuelPT
            sensorArray.at(13)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegLoxPT
            sensorArray.at(14)->setState(SensorState::Medium);        // Renegade SF Stand - FuelTankPT
            sensorArray.at(15)->setState(SensorState::Medium);        // Renegade SF Stand - LoxTankPT
            sensorArray.at(16)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressFuelPT
            sensorArray.at(17)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressLoxPT
            sensorArray.at(18)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode2
            sensorArray.at(19)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode3
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireChamberPT1
            sensorArray.at(21)->setState(SensorState::Medium);        // Pasafire - PasafireFuelTankPT
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireLOXTankPT
            sensorArray.at(23)->setState(SensorState::Medium);        // Pasafire - PasafireFuelRegPT
            sensorArray.at(24)->setState(SensorState::Medium);        // Pasafire - PasafireLOXRegPT
            sensorArray.at(25)->setState(SensorState::Medium);        // Pasafire - PasafireFuelLinePT
            sensorArray.at(26)->setState(SensorState::Medium);        // Pasafire - PasafireLOXLinePT
            sensorArray.at(27)->setState(SensorState::Medium);        // Pasafire - PasafirePneumaticPT
            sensorArray.at(28)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1pos
            sensorArray.at(29)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1neg
            sensorArray.at(30)->setState(SensorState::Medium);        // Pasafire - MCUtempNode8
            currentState = State::HiPressArm;
            }
            break;
        case command_HiPressPressurized:
            if(currentState == State::HiPressArm || currentState == State::TankPressArm) //added second conditional to allow entry backwards in a "disarm" state change
            {
            valveArray.at(0)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(10)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::CloseCommanded);        // Pasafire
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::Off);
            valveEnableArray.at(4)->setState(ValveEnableState::Off);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
            valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);  // Renegade SF Stand
            autoSequenceArray.at(1)->setState(AutoSequenceState::Standby);  // Pasafire
            sensorArray.at(0)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1pos
            sensorArray.at(1)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1neg
            sensorArray.at(2)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2pos
            sensorArray.at(3)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2neg
            sensorArray.at(4)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3pos
            sensorArray.at(5)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3neg
            sensorArray.at(6)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT2
            sensorArray.at(7)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT1
            sensorArray.at(8)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInletPropSidePT
            sensorArray.at(9)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInjectorPT
            sensorArray.at(10)->setState(SensorState::Medium);        // Renegade SF Stand - LoxInletPropSidePT
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand - MVPneumaticsPT
            sensorArray.at(12)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegFuelPT
            sensorArray.at(13)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegLoxPT
            sensorArray.at(14)->setState(SensorState::Medium);        // Renegade SF Stand - FuelTankPT
            sensorArray.at(15)->setState(SensorState::Medium);        // Renegade SF Stand - LoxTankPT
            sensorArray.at(16)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressFuelPT
            sensorArray.at(17)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressLoxPT
            sensorArray.at(18)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode2
            sensorArray.at(19)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode3
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireChamberPT1
            sensorArray.at(21)->setState(SensorState::Medium);        // Pasafire - PasafireFuelTankPT
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireLOXTankPT
            sensorArray.at(23)->setState(SensorState::Medium);        // Pasafire - PasafireFuelRegPT
            sensorArray.at(24)->setState(SensorState::Medium);        // Pasafire - PasafireLOXRegPT
            sensorArray.at(25)->setState(SensorState::Medium);        // Pasafire - PasafireFuelLinePT
            sensorArray.at(26)->setState(SensorState::Medium);        // Pasafire - PasafireLOXLinePT
            sensorArray.at(27)->setState(SensorState::Medium);        // Pasafire - PasafirePneumaticPT
            sensorArray.at(28)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1pos
            sensorArray.at(29)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1neg
            sensorArray.at(30)->setState(SensorState::Medium);        // Pasafire - MCUtempNode8
            currentState = State::HiPressPressurized;
            }
            break;
        case command_TankPressArm:
            if(currentState == State::HiPressPressurized)
            {
            valveArray.at(0)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(10)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::CloseCommanded);        // Pasafire
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
            valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::On);        // Pasafire
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);  // Renegade SF Stand
            autoSequenceArray.at(1)->setState(AutoSequenceState::Standby);  // Pasafire
            sensorArray.at(0)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1pos
            sensorArray.at(1)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1neg
            sensorArray.at(2)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2pos
            sensorArray.at(3)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2neg
            sensorArray.at(4)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3pos
            sensorArray.at(5)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3neg
            sensorArray.at(6)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT2
            sensorArray.at(7)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT1
            sensorArray.at(8)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInletPropSidePT
            sensorArray.at(9)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInjectorPT
            sensorArray.at(10)->setState(SensorState::Medium);        // Renegade SF Stand - LoxInletPropSidePT
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand - MVPneumaticsPT
            sensorArray.at(12)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegFuelPT
            sensorArray.at(13)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegLoxPT
            sensorArray.at(14)->setState(SensorState::Fast);        // Renegade SF Stand - FuelTankPT
            sensorArray.at(15)->setState(SensorState::Fast);        // Renegade SF Stand - LoxTankPT
            sensorArray.at(16)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressFuelPT
            sensorArray.at(17)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressLoxPT
            sensorArray.at(18)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode2
            sensorArray.at(19)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode3
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireChamberPT1
            sensorArray.at(21)->setState(SensorState::Medium);        // Pasafire - PasafireFuelTankPT
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireLOXTankPT
            sensorArray.at(23)->setState(SensorState::Medium);        // Pasafire - PasafireFuelRegPT
            sensorArray.at(24)->setState(SensorState::Medium);        // Pasafire - PasafireLOXRegPT
            sensorArray.at(25)->setState(SensorState::Medium);        // Pasafire - PasafireFuelLinePT
            sensorArray.at(26)->setState(SensorState::Medium);        // Pasafire - PasafireLOXLinePT
            sensorArray.at(27)->setState(SensorState::Medium);        // Pasafire - PasafirePneumaticPT
            sensorArray.at(28)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1pos
            sensorArray.at(29)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1neg
            sensorArray.at(30)->setState(SensorState::Medium);        // Pasafire - MCUtempNode8
            currentState = State::TankPressArm;
            }
            break;
        case commend_TankPressPressurized:
            if(currentState == State::TankPressArm)
            {
            valveArray.at(0)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(10)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::OpenCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::OpenCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::CloseCommanded);        // Pasafire
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
            valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::On);        // Pasafire
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);  // Renegade SF Stand
            autoSequenceArray.at(1)->setState(AutoSequenceState::Standby);  // Pasafire
            sensorArray.at(0)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1pos
            sensorArray.at(1)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell1neg
            sensorArray.at(2)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2pos
            sensorArray.at(3)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell2neg
            sensorArray.at(4)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3pos
            sensorArray.at(5)->setState(SensorState::Medium);         // Renegade SF Stand - ThrustMountLoadCell3neg
            sensorArray.at(6)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT2
            sensorArray.at(7)->setState(SensorState::Medium);         // Renegade SF Stand - ChamberPT1
            sensorArray.at(8)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInletPropSidePT
            sensorArray.at(9)->setState(SensorState::Medium);         // Renegade SF Stand - FuelInjectorPT
            sensorArray.at(10)->setState(SensorState::Medium);        // Renegade SF Stand - LoxInletPropSidePT
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand - MVPneumaticsPT
            sensorArray.at(12)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegFuelPT
            sensorArray.at(13)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegLoxPT
            sensorArray.at(14)->setState(SensorState::Fast);        // Renegade SF Stand - FuelTankPT
            sensorArray.at(15)->setState(SensorState::Fast);        // Renegade SF Stand - LoxTankPT
            sensorArray.at(16)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressFuelPT
            sensorArray.at(17)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressLoxPT
            sensorArray.at(18)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode2
            sensorArray.at(19)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode3
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireChamberPT1
            sensorArray.at(21)->setState(SensorState::Medium);        // Pasafire - PasafireFuelTankPT
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire - PasafireLOXTankPT
            sensorArray.at(23)->setState(SensorState::Medium);        // Pasafire - PasafireFuelRegPT
            sensorArray.at(24)->setState(SensorState::Medium);        // Pasafire - PasafireLOXRegPT
            sensorArray.at(25)->setState(SensorState::Medium);        // Pasafire - PasafireFuelLinePT
            sensorArray.at(26)->setState(SensorState::Medium);        // Pasafire - PasafireLOXLinePT
            sensorArray.at(27)->setState(SensorState::Medium);        // Pasafire - PasafirePneumaticPT
            sensorArray.at(28)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1pos
            sensorArray.at(29)->setState(SensorState::Medium);        // Pasafire - PasafireThrustMountLoadCell1neg
            sensorArray.at(30)->setState(SensorState::Medium);        // Pasafire - MCUtempNode8
            currentState = State::TankPressPressurized;
            }
            break;
        case command_fireArm:
            if(currentState == State::TankPressPressurized)
            {
            valveArray.at(0)->setState(ValveState::CloseCommanded);          // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(10)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::OpenCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::OpenCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::CloseCommanded);        // Pasafire
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2         
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2
            valveEnableArray.at(0)->setState(ValveEnableState::Off);
            valveEnableArray.at(1)->setState(ValveEnableState::On);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
            valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::On);        // Pasafire
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);  // Renegade SF Stand
            autoSequenceArray.at(1)->setState(AutoSequenceState::Standby);  // Pasafire
            sensorArray.at(0)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell1pos
            sensorArray.at(1)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell1neg
            sensorArray.at(2)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell2pos
            sensorArray.at(3)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell2neg
            sensorArray.at(4)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell3pos
            sensorArray.at(5)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell3neg
            sensorArray.at(6)->setState(SensorState::Fast);         // Renegade SF Stand - ChamberPT2
            sensorArray.at(7)->setState(SensorState::Fast);         // Renegade SF Stand - ChamberPT1
            sensorArray.at(8)->setState(SensorState::Fast);         // Renegade SF Stand - FuelInletPropSidePT
            sensorArray.at(9)->setState(SensorState::Fast);         // Renegade SF Stand - FuelInjectorPT
            sensorArray.at(10)->setState(SensorState::Fast);        // Renegade SF Stand - LoxInletPropSidePT
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand - MVPneumaticsPT
            sensorArray.at(12)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegFuelPT
            sensorArray.at(13)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegLoxPT
            sensorArray.at(14)->setState(SensorState::Fast);        // Renegade SF Stand - FuelTankPT
            sensorArray.at(15)->setState(SensorState::Fast);        // Renegade SF Stand - LoxTankPT
            sensorArray.at(16)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressFuelPT
            sensorArray.at(17)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressLoxPT
            sensorArray.at(18)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode2
            sensorArray.at(19)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode3
            sensorArray.at(20)->setState(SensorState::Fast);        // Pasafire - PasafireChamberPT1
            sensorArray.at(21)->setState(SensorState::Fast);        // Pasafire - PasafireFuelTankPT
            sensorArray.at(20)->setState(SensorState::Fast);        // Pasafire - PasafireLOXTankPT
            sensorArray.at(23)->setState(SensorState::Fast);        // Pasafire - PasafireFuelRegPT
            sensorArray.at(24)->setState(SensorState::Fast);        // Pasafire - PasafireLOXRegPT
            sensorArray.at(25)->setState(SensorState::Fast);        // Pasafire - PasafireFuelLinePT
            sensorArray.at(26)->setState(SensorState::Fast);        // Pasafire - PasafireLOXLinePT
            sensorArray.at(27)->setState(SensorState::Medium);        // Pasafire - PasafirePneumaticPT
            sensorArray.at(28)->setState(SensorState::Fast);        // Pasafire - PasafireThrustMountLoadCell1pos
            sensorArray.at(29)->setState(SensorState::Fast);        // Pasafire - PasafireThrustMountLoadCell1neg
            sensorArray.at(30)->setState(SensorState::Medium);        // Pasafire - MCUtempNode8
            currentState = State::fireArmed;
            }
            break;
        case command_fire:
            if(currentState == State::fireArmed)
            {
            valveArray.at(0)->setState(ValveState::CloseCommanded);          // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::FireCommanded);          // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::FireCommanded);          // Renegade SF Stand
            valveArray.at(10)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::OpenCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::OpenCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::FireCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::FireCommanded);        // Pasafire
            pyroArray.at(0)->setState(PyroState::FireCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::FireCommanded);             // Renegade SF Igniter2
            pyroArray.at(2)->setState(PyroState::FireCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::FireCommanded);             // Pasafire Igniter2
            valveEnableArray.at(0)->setState(ValveEnableState::Off);
            valveEnableArray.at(1)->setState(ValveEnableState::On);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
            valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::On);        // Pasafire
            autoSequenceArray.at(0)->setState(AutoSequenceState::RunCommanded);     // Renegade SF Stand
            autoSequenceArray.at(1)->setState(AutoSequenceState::RunCommanded);     // Pasafire
            sensorArray.at(0)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell1pos
            sensorArray.at(1)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell1neg
            sensorArray.at(2)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell2pos
            sensorArray.at(3)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell2neg
            sensorArray.at(4)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell3pos
            sensorArray.at(5)->setState(SensorState::Fast);         // Renegade SF Stand - ThrustMountLoadCell3neg
            sensorArray.at(6)->setState(SensorState::Fast);         // Renegade SF Stand - ChamberPT2
            sensorArray.at(7)->setState(SensorState::Fast);         // Renegade SF Stand - ChamberPT1
            sensorArray.at(8)->setState(SensorState::Fast);         // Renegade SF Stand - FuelInletPropSidePT
            sensorArray.at(9)->setState(SensorState::Fast);         // Renegade SF Stand - FuelInjectorPT
            sensorArray.at(10)->setState(SensorState::Fast);        // Renegade SF Stand - LoxInletPropSidePT
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand - MVPneumaticsPT
            sensorArray.at(12)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegFuelPT
            sensorArray.at(13)->setState(SensorState::Fast);        // Renegade SF Stand - DomeRegLoxPT
            sensorArray.at(14)->setState(SensorState::Fast);        // Renegade SF Stand - FuelTankPT
            sensorArray.at(15)->setState(SensorState::Fast);        // Renegade SF Stand - LoxTankPT
            sensorArray.at(16)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressFuelPT
            sensorArray.at(17)->setState(SensorState::Fast);        // Renegade SF Stand - HiPressLoxPT
            sensorArray.at(18)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode2
            sensorArray.at(19)->setState(SensorState::Medium);        // Renegade SF Stand - MCUtempNode3
            sensorArray.at(20)->setState(SensorState::Fast);        // Pasafire - PasafireChamberPT1
            sensorArray.at(21)->setState(SensorState::Fast);        // Pasafire - PasafireFuelTankPT
            sensorArray.at(20)->setState(SensorState::Fast);        // Pasafire - PasafireLOXTankPT
            sensorArray.at(23)->setState(SensorState::Fast);        // Pasafire - PasafireFuelRegPT
            sensorArray.at(24)->setState(SensorState::Fast);        // Pasafire - PasafireLOXRegPT
            sensorArray.at(25)->setState(SensorState::Fast);        // Pasafire - PasafireFuelLinePT
            sensorArray.at(26)->setState(SensorState::Fast);        // Pasafire - PasafireLOXLinePT
            sensorArray.at(27)->setState(SensorState::Medium);        // Pasafire - PasafirePneumaticPT
            sensorArray.at(28)->setState(SensorState::Fast);        // Pasafire - PasafireThrustMountLoadCell1pos
            sensorArray.at(29)->setState(SensorState::Fast);        // Pasafire - PasafireThrustMountLoadCell1neg
            sensorArray.at(30)->setState(SensorState::Medium);        // Pasafire - MCUtempNode8
            currentState = State::fire;
            }
            break;


        case command_closeHiPress:
            if(currentState == State::test)
            {
                valveArray.at(0)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(0)->setState(ValveState::CloseCommanded);
            }
            break;
        case command_openHiPress:
             if(currentState == State::test)
            {
                valveArray.at(0)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(0)->setState(ValveState::OpenCommanded);
            }
            break;
        case command_closeHiPressVent:
            if(currentState == State::test)
            {
                valveArray.at(1)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(1)->setState(ValveState::CloseCommanded);
            }            
            break;
        case command_openHiPressVent:
             if(currentState == State::test)
            {
                valveArray.at(1)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(1)->setState(ValveState::OpenCommanded);
            }              
            break;
        case command_closeLoxVent:
            if(currentState == State::test)
            {
                valveArray.at(2)->setState(ValveState::CloseCommanded);
                valveArray.at(10)->setState(ValveState::CloseCommanded);
                valveEnableArray.at(5)->setState(ValveEnableState::On);        // Renegade SF Stand                        
                valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire Stand                        
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(2)->setState(ValveState::CloseCommanded);
                valveArray.at(10)->setState(ValveState::CloseCommanded);
                valveEnableArray.at(5)->setState(ValveEnableState::On);        // Renegade SF Stand                        
                valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire Stand                        
            }              
            break;
        case command_openLoxVent:
             if(currentState == State::test)
            {
                valveArray.at(2)->setState(ValveState::OpenCommanded);
                valveArray.at(10)->setState(ValveState::OpenCommanded);
                valveEnableArray.at(5)->setState(ValveEnableState::Off);        // Renegade SF Stand                        
                valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire Stand                        
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(2)->setState(ValveState::OpenCommanded);
                valveArray.at(10)->setState(ValveState::OpenCommanded);
                valveEnableArray.at(5)->setState(ValveEnableState::Off);        // Renegade SF Stand                        
                valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire Stand                        
            }              
            break;
        case command_closeLoxDomeReg:
            if(currentState == State::test)
            {
                valveArray.at(3)->setState(ValveState::CloseCommanded);
                valveArray.at(11)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(3)->setState(ValveState::CloseCommanded);
                valveArray.at(11)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openLoxDomeReg:
             if(currentState == State::test)
            {
                valveArray.at(3)->setState(ValveState::OpenCommanded);
                valveArray.at(11)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(3)->setState(ValveState::OpenCommanded);
                valveArray.at(11)->setState(ValveState::OpenCommanded);
            }              
            break; 
        case command_closeLoxDomeRegVent:
            if(currentState == State::test)
            {
                valveArray.at(4)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(4)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openLoxDomeRegVent:
             if(currentState == State::test)
            {
                valveArray.at(4)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(4)->setState(ValveState::OpenCommanded);
            }              
            break; 
        case command_closeFuelVent:
            if(currentState == State::test)
            {
                valveArray.at(5)->setState(ValveState::CloseCommanded);
                valveArray.at(12)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(5)->setState(ValveState::CloseCommanded);
                valveArray.at(12)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openFuelVent:
             if(currentState == State::test)
            {
                valveArray.at(5)->setState(ValveState::OpenCommanded);
                valveArray.at(12)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(5)->setState(ValveState::OpenCommanded);
                valveArray.at(12)->setState(ValveState::OpenCommanded);
            }              
            break;
        case command_closeFuelDomeReg:
            if(currentState == State::test)
            {
                valveArray.at(6)->setState(ValveState::CloseCommanded);
                valveArray.at(13)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(6)->setState(ValveState::CloseCommanded);
                valveArray.at(13)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openFuelDomeReg:
             if(currentState == State::test)
            {
                valveArray.at(6)->setState(ValveState::OpenCommanded);
                valveArray.at(13)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(6)->setState(ValveState::OpenCommanded);
                valveArray.at(13)->setState(ValveState::OpenCommanded);
            }              
            break; 
        case command_closeFuelDomeRegVent:
            if(currentState == State::test)
            {
                valveArray.at(7)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(7)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openFuelDomeRegVent:
             if(currentState == State::test)
            {
                valveArray.at(7)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(7)->setState(ValveState::OpenCommanded);
            }              
            break; 
        case command_closeFuelMV:
            if(currentState == State::test)
            {
                valveArray.at(8)->setState(ValveState::CloseCommanded);
                valveArray.at(15)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(8)->setState(ValveState::CloseCommanded);
                valveArray.at(15)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openFuelMV:
             if(currentState == State::test)
            {
                valveArray.at(8)->setState(ValveState::OpenCommanded);
                valveArray.at(15)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(8)->setState(ValveState::OpenCommanded);
                valveArray.at(15)->setState(ValveState::OpenCommanded);
            }              
            break;
        case command_closeLoxMV:
            if(currentState == State::test)
            {
                valveArray.at(9)->setState(ValveState::CloseCommanded);
                valveArray.at(14)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(9)->setState(ValveState::CloseCommanded);
                valveArray.at(14)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openLoxMV:
             if(currentState == State::test)
            {
                valveArray.at(9)->setState(ValveState::OpenCommanded);
                valveArray.at(14)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(9)->setState(ValveState::OpenCommanded);
                valveArray.at(14)->setState(ValveState::OpenCommanded);
            }              
            break;

        case command_disableHiPressHiVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(0)->setState(ValveEnableState::Off);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(0)->setState(ValveEnableState::Off);
            }              
            break;
        case command_enableHiPressHiVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(0)->setState(ValveEnableState::On);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(0)->setState(ValveEnableState::On);
            }              
            break;
        case command_disableFuelVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(1)->setState(ValveEnableState::Off);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(1)->setState(ValveEnableState::Off);
            }              
            break;
        case command_enableFuelVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(1)->setState(ValveEnableState::On);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(1)->setState(ValveEnableState::On);
            }              
            break;
        case command_disableLoxDomeRegLoxDomeVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(2)->setState(ValveEnableState::Off);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(2)->setState(ValveEnableState::Off);
            }              
            break;
        case command_enableLoxDomeRegLoxDomeVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(2)->setState(ValveEnableState::On);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(2)->setState(ValveEnableState::On);
            }              
            break;
        case command_disableFuelDomeRegFuelDomeVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(3)->setState(ValveEnableState::Off);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(3)->setState(ValveEnableState::Off);
            }              
            break;
        case command_enableFuelDomeRegFuelDomeVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(3)->setState(ValveEnableState::On);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(3)->setState(ValveEnableState::On);
            }              
            break;
        case command_disableLoxVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(4)->setState(ValveEnableState::Off);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(4)->setState(ValveEnableState::Off);
            }              
            break;
        case command_enableLoxVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(4)->setState(ValveEnableState::On);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(4)->setState(ValveEnableState::On);
            }              
            break;
        case command_disableMainValvesSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(5)->setState(ValveEnableState::Off);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(5)->setState(ValveEnableState::Off);
            }              
            break;
        case command_enableMainValvesSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(5)->setState(ValveEnableState::On);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(5)->setState(ValveEnableState::On);
            }              
            break;
        case command_engineIgniterPyro1_Off:
            if(currentState == State::test)
            {
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            }
            else if (currentState == State::offNominal)
            {
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            }              
            break;
        case command_engineIgniterPyro1_On:
            if(currentState == State::test)
            {
            pyroArray.at(0)->setState(PyroState::OnCommanded);             // Renegade SF Igniter1
            pyroArray.at(2)->setState(PyroState::OnCommanded);             // Pasafire Igniter1
            }
            else if (currentState == State::offNominal)
            {
            pyroArray.at(0)->setState(PyroState::OnCommanded);             // Renegade SF Igniter1
            pyroArray.at(2)->setState(PyroState::OnCommanded);             // Pasafire Igniter1
            }              
            break;
        case command_engineIgniterPyro2_Off:
            if(currentState == State::test)
            {
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            }
            else if (currentState == State::offNominal)
            {
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            }              
            break;
        case command_engineIgniterPyro2_On:
            if(currentState == State::test)
            {
            pyroArray.at(1)->setState(PyroState::OnCommanded);             // Renegade SF Igniter1
            pyroArray.at(3)->setState(PyroState::OnCommanded);             // Pasafire Igniter1
            }
            else if (currentState == State::offNominal)
            {
            pyroArray.at(1)->setState(PyroState::OnCommanded);             // Renegade SF Igniter1
            pyroArray.at(3)->setState(PyroState::OnCommanded);             // Pasafire Igniter1
            }              
            break;

        case command_allSensorsOff:
            if(currentState == State::test)
            {
            sensorArray.at(0)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(1)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(2)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(3)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(4)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(5)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(6)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(7)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(8)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(9)->setState(SensorState::Off);         // Renegade SF Stand
            sensorArray.at(10)->setState(SensorState::Off);        // Renegade SF Stand
            sensorArray.at(11)->setState(SensorState::Off);        // Renegade SF Stand
            sensorArray.at(12)->setState(SensorState::Off);        // Renegade SF Stand
            sensorArray.at(13)->setState(SensorState::Off);        // Renegade SF Stand
            sensorArray.at(14)->setState(SensorState::Off);        // Renegade SF Stand
            sensorArray.at(15)->setState(SensorState::Off);        // Renegade SF Stand
            sensorArray.at(16)->setState(SensorState::Off);        // Renegade SF Stand
            sensorArray.at(17)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(18)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(19)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(20)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(21)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(22)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(23)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(24)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(25)->setState(SensorState::Off);        // Pasafire
            sensorArray.at(26)->setState(SensorState::Off);        // Pasafire
            }            
            break;
        case command_allSensorsSlow:
            if(currentState == State::test)
            {
            sensorArray.at(0)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(1)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(2)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(3)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(4)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(5)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(6)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(7)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(8)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(9)->setState(SensorState::Slow);         // Renegade SF Stand
            sensorArray.at(10)->setState(SensorState::Slow);        // Renegade SF Stand
            sensorArray.at(11)->setState(SensorState::Slow);        // Renegade SF Stand
            sensorArray.at(12)->setState(SensorState::Slow);        // Renegade SF Stand
            sensorArray.at(13)->setState(SensorState::Slow);        // Renegade SF Stand
            sensorArray.at(14)->setState(SensorState::Slow);        // Renegade SF Stand
            sensorArray.at(15)->setState(SensorState::Slow);        // Renegade SF Stand
            sensorArray.at(16)->setState(SensorState::Slow);        // Renegade SF Stand
            sensorArray.at(17)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(18)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(19)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(20)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(21)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(22)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(23)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(24)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(25)->setState(SensorState::Slow);        // Pasafire
            sensorArray.at(26)->setState(SensorState::Slow);        // Pasafire
            }             
            break;
        case command_allSensorsMedium:
            if(currentState == State::test)
            {
            sensorArray.at(0)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(1)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(2)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(3)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(4)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(5)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(6)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(7)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(8)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(9)->setState(SensorState::Medium);         // Renegade SF Stand
            sensorArray.at(10)->setState(SensorState::Medium);        // Renegade SF Stand
            sensorArray.at(11)->setState(SensorState::Medium);        // Renegade SF Stand
            sensorArray.at(12)->setState(SensorState::Medium);        // Renegade SF Stand
            sensorArray.at(13)->setState(SensorState::Medium);        // Renegade SF Stand
            sensorArray.at(14)->setState(SensorState::Medium);        // Renegade SF Stand
            sensorArray.at(15)->setState(SensorState::Medium);        // Renegade SF Stand
            sensorArray.at(16)->setState(SensorState::Medium);        // Renegade SF Stand
            sensorArray.at(17)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(18)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(19)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(20)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(21)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(22)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(23)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(24)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(25)->setState(SensorState::Medium);        // Pasafire
            sensorArray.at(26)->setState(SensorState::Medium);        // Pasafire
            }             
            break;
        case command_allSensorsFast:
            if(currentState == State::test)
            {
            sensorArray.at(0)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(1)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(2)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(3)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(4)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(5)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(6)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(7)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(8)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(9)->setState(SensorState::Fast);         // Renegade SF Stand
            sensorArray.at(10)->setState(SensorState::Fast);        // Renegade SF Stand
            sensorArray.at(11)->setState(SensorState::Fast);        // Renegade SF Stand
            sensorArray.at(12)->setState(SensorState::Fast);        // Renegade SF Stand
            sensorArray.at(13)->setState(SensorState::Fast);        // Renegade SF Stand
            sensorArray.at(14)->setState(SensorState::Fast);        // Renegade SF Stand
            sensorArray.at(15)->setState(SensorState::Fast);        // Renegade SF Stand
            sensorArray.at(16)->setState(SensorState::Fast);        // Renegade SF Stand
            sensorArray.at(17)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(18)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(19)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(20)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(21)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(22)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(23)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(24)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(25)->setState(SensorState::Fast);        // Pasafire
            sensorArray.at(26)->setState(SensorState::Fast);        // Pasafire
            }            
            break;
        case command_allSensorsCalibration:
            if(currentState == State::test)
            {
            sensorArray.at(0)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(1)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(2)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(3)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(4)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(5)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(6)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(7)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(8)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(9)->setState(SensorState::Calibration);         // Renegade SF Stand
            sensorArray.at(10)->setState(SensorState::Calibration);        // Renegade SF Stand
            sensorArray.at(11)->setState(SensorState::Calibration);        // Renegade SF Stand
            sensorArray.at(12)->setState(SensorState::Calibration);        // Renegade SF Stand
            sensorArray.at(13)->setState(SensorState::Calibration);        // Renegade SF Stand
            sensorArray.at(14)->setState(SensorState::Calibration);        // Renegade SF Stand
            sensorArray.at(15)->setState(SensorState::Calibration);        // Renegade SF Stand
            sensorArray.at(16)->setState(SensorState::Calibration);        // Renegade SF Stand
            sensorArray.at(17)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(18)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(19)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(20)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(21)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(22)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(23)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(24)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(25)->setState(SensorState::Calibration);        // Pasafire
            sensorArray.at(26)->setState(SensorState::Calibration);        // Pasafire
            }            
            break;
        default:
            break;
    }
}