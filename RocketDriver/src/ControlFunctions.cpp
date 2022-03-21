#include "ControlFunctions.h"
//#include "pinList.h"


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
        currentCommand = command_vent;
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
/*         valveArray.at(14)->setState(ValveState::CloseCommanded);    // Pasafire MV
        valveArray.at(15)->setState(ValveState::CloseCommanded);    // Pasafire MV */
        pyroArray.at(0)->setState(PyroState::OffCommanded);         // Renegade SF Igniter1
        pyroArray.at(1)->setState(PyroState::OffCommanded);         // Renegade SF Igniter2
/*         pyroArray.at(2)->setState(PyroState::OffCommanded);         // Pasafire Igniter1
        pyroArray.at(3)->setState(PyroState::OffCommanded);         // Pasafire Igniter2 */
        valveEnableArray.at(0)->setState(ValveEnableState::Off);    // Renegade SF Stand MV Enable
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
/*             valveArray.at(10)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(11)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(12)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(13)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(14)->setState(ValveState::CloseCommanded);        // Pasafire
            valveArray.at(15)->setState(ValveState::CloseCommanded);        // Pasafire */
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2
/*             pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2 */
            valveEnableArray.at(0)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(1)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(2)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(3)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(4)->setState(ValveEnableState::Off);        // Renegade SF Stand
            valveEnableArray.at(5)->setState(ValveEnableState::Off);        // Renegade SF Stand
/*             valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire */
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);  // Global? Not sure how I want to handle multinode autosequence yet
            sensorArray.at(0)->setState(SensorState::Slow);
            sensorArray.at(1)->setState(SensorState::Slow);
            sensorArray.at(2)->setState(SensorState::Slow);
            sensorArray.at(3)->setState(SensorState::Slow);
            sensorArray.at(4)->setState(SensorState::Slow);
            sensorArray.at(5)->setState(SensorState::Slow);
            sensorArray.at(6)->setState(SensorState::Slow);
            sensorArray.at(7)->setState(SensorState::Slow);
            sensorArray.at(8)->setState(SensorState::Slow);
            sensorArray.at(9)->setState(SensorState::Slow);
            sensorArray.at(10)->setState(SensorState::Slow);
            sensorArray.at(11)->setState(SensorState::Slow);
            sensorArray.at(12)->setState(SensorState::Slow);
            sensorArray.at(13)->setState(SensorState::Slow);
            sensorArray.at(14)->setState(SensorState::Slow);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow);
            currentState = State::passive;
            haltFlag = false;
            break;
        case command_test:
            if(currentState == State::passive)
            {
            currentState = State::test;
/*             valveEnableArray.at(0)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(1)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(2)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(3)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(4)->setState(ValveEnableState::On);        // Renegade SF Stand
            valveEnableArray.at(5)->setState(ValveEnableState::On);        // Renegade SF Stand                        
            valveEnableArray.at(6)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::On);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::On);        // Pasafire */
/*             sensorArray.at(0)->setState(SensorState::Slow);
            sensorArray.at(1)->setState(SensorState::Slow);
            sensorArray.at(2)->setState(SensorState::Slow);
            sensorArray.at(3)->setState(SensorState::Slow);
            sensorArray.at(4)->setState(SensorState::Slow);
            sensorArray.at(5)->setState(SensorState::Slow);
            sensorArray.at(6)->setState(SensorState::Slow);
            sensorArray.at(7)->setState(SensorState::Slow);
            sensorArray.at(8)->setState(SensorState::Slow);
            sensorArray.at(9)->setState(SensorState::Slow);
            sensorArray.at(10)->setState(SensorState::Slow);
            sensorArray.at(11)->setState(SensorState::Slow);
            sensorArray.at(12)->setState(SensorState::Slow);
            sensorArray.at(13)->setState(SensorState::Slow);
            sensorArray.at(14)->setState(SensorState::Slow);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow); */
            }
            break;
        case command_EnterOffNominal:
            priorState = currentState; //for remembering the state the system was in when entering Off Nominal
            currentState = State::offNominal;
/*             valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::On);
            valveEnableArray.at(2)->setState(ValveEnableState::On);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);   */
            break;            
        case command_ExitOffNominal:
            if(currentState == State::offNominal)
            {
            currentState = priorState;
/*             valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::On);
            valveEnableArray.at(2)->setState(ValveEnableState::On);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);   */
            }
            break;        
        case command_abort:
            haltFlag = true;
            currentState = State::abort;
            autoSequenceArray.at(0)->setState(AutoSequenceState::Hold);
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
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
/*             pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2 */
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::On);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::Off);
/*             valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire */
            autoSequenceArray.at(0)->setState(AutoSequenceState::Hold);            
            sensorArray.at(0)->setState(SensorState::Medium);
            sensorArray.at(1)->setState(SensorState::Medium);
            sensorArray.at(2)->setState(SensorState::Medium);
            sensorArray.at(3)->setState(SensorState::Slow);
            sensorArray.at(4)->setState(SensorState::Slow);
            sensorArray.at(5)->setState(SensorState::Slow);
            sensorArray.at(6)->setState(SensorState::Slow);
            sensorArray.at(7)->setState(SensorState::Slow);
            sensorArray.at(8)->setState(SensorState::Medium);
            sensorArray.at(9)->setState(SensorState::Medium);
            sensorArray.at(10)->setState(SensorState::Medium);
            sensorArray.at(11)->setState(SensorState::Fast);
            sensorArray.at(12)->setState(SensorState::Fast);
            sensorArray.at(13)->setState(SensorState::Fast);
            sensorArray.at(14)->setState(SensorState::Medium);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow);
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
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
/*             pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2 */
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::Off);
            valveEnableArray.at(4)->setState(ValveEnableState::Off);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
/*             valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire */
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);
            sensorArray.at(0)->setState(SensorState::Medium);
            sensorArray.at(1)->setState(SensorState::Medium);
            sensorArray.at(2)->setState(SensorState::Medium);
            sensorArray.at(3)->setState(SensorState::Slow);
            sensorArray.at(4)->setState(SensorState::Slow);
            sensorArray.at(5)->setState(SensorState::Slow);
            sensorArray.at(6)->setState(SensorState::Slow);
            sensorArray.at(7)->setState(SensorState::Slow);
            sensorArray.at(8)->setState(SensorState::Medium);
            sensorArray.at(9)->setState(SensorState::Fast);
            sensorArray.at(10)->setState(SensorState::Fast);
            sensorArray.at(11)->setState(SensorState::Medium);
            sensorArray.at(12)->setState(SensorState::Medium);
            sensorArray.at(13)->setState(SensorState::Fast);
            sensorArray.at(14)->setState(SensorState::Fast);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow);
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
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
/*             pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2 */
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::Off);
            valveEnableArray.at(4)->setState(ValveEnableState::Off);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
/*             valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire */
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);
            sensorArray.at(0)->setState(SensorState::Medium);
            sensorArray.at(1)->setState(SensorState::Medium);
            sensorArray.at(2)->setState(SensorState::Medium);
            sensorArray.at(3)->setState(SensorState::Slow);
            sensorArray.at(4)->setState(SensorState::Slow);
            sensorArray.at(5)->setState(SensorState::Slow);
            sensorArray.at(6)->setState(SensorState::Slow);
            sensorArray.at(7)->setState(SensorState::Slow);
            sensorArray.at(8)->setState(SensorState::Medium);
            sensorArray.at(9)->setState(SensorState::Fast);
            sensorArray.at(10)->setState(SensorState::Fast);
            sensorArray.at(11)->setState(SensorState::Medium);
            sensorArray.at(12)->setState(SensorState::Medium);
            sensorArray.at(13)->setState(SensorState::Fast);
            sensorArray.at(14)->setState(SensorState::Fast);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow);
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
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
/*             pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2 */
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
/*             valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire */
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);
            sensorArray.at(0)->setState(SensorState::Medium);
            sensorArray.at(1)->setState(SensorState::Medium);
            sensorArray.at(2)->setState(SensorState::Medium);
            sensorArray.at(3)->setState(SensorState::Slow);
            sensorArray.at(4)->setState(SensorState::Slow);
            sensorArray.at(5)->setState(SensorState::Slow);
            sensorArray.at(6)->setState(SensorState::Slow);
            sensorArray.at(7)->setState(SensorState::Slow);
            sensorArray.at(8)->setState(SensorState::Medium);
            sensorArray.at(9)->setState(SensorState::Medium);
            sensorArray.at(10)->setState(SensorState::Medium);
            sensorArray.at(11)->setState(SensorState::Fast);
            sensorArray.at(12)->setState(SensorState::Fast);
            sensorArray.at(13)->setState(SensorState::Fast);
            sensorArray.at(14)->setState(SensorState::Medium);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow);
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
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2            
/*             pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2 */
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::Off);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
/*             valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire */
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);
            sensorArray.at(0)->setState(SensorState::Medium);
            sensorArray.at(1)->setState(SensorState::Medium);
            sensorArray.at(2)->setState(SensorState::Medium);
            sensorArray.at(3)->setState(SensorState::Slow);
            sensorArray.at(4)->setState(SensorState::Slow);
            sensorArray.at(5)->setState(SensorState::Slow);
            sensorArray.at(6)->setState(SensorState::Slow);
            sensorArray.at(7)->setState(SensorState::Slow);
            sensorArray.at(8)->setState(SensorState::Medium);
            sensorArray.at(9)->setState(SensorState::Medium);
            sensorArray.at(10)->setState(SensorState::Medium);
            sensorArray.at(11)->setState(SensorState::Fast);
            sensorArray.at(12)->setState(SensorState::Fast);
            sensorArray.at(13)->setState(SensorState::Fast);
            sensorArray.at(14)->setState(SensorState::Medium);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow);
            currentState = State::TankPressPressurized;
            }
            break;
        case command_fireArm:
            if(currentState == State::TankPressPressurized)
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
            pyroArray.at(0)->setState(PyroState::OffCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::OffCommanded);             // Renegade SF Igniter2         
/*             pyroArray.at(2)->setState(PyroState::OffCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::OffCommanded);             // Pasafire Igniter2 */
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::On);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
/*             valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire */
            autoSequenceArray.at(0)->setState(AutoSequenceState::Standby);
            sensorArray.at(0)->setState(SensorState::Fast);
            sensorArray.at(1)->setState(SensorState::Fast);
            sensorArray.at(2)->setState(SensorState::Fast);
            sensorArray.at(3)->setState(SensorState::Fast);
            sensorArray.at(4)->setState(SensorState::Fast);
            sensorArray.at(5)->setState(SensorState::Fast);
            sensorArray.at(6)->setState(SensorState::Fast);
            sensorArray.at(7)->setState(SensorState::Fast);
            sensorArray.at(8)->setState(SensorState::Medium);
            sensorArray.at(9)->setState(SensorState::Medium);
            sensorArray.at(10)->setState(SensorState::Medium);
            sensorArray.at(11)->setState(SensorState::Fast);
            sensorArray.at(12)->setState(SensorState::Fast);
            sensorArray.at(13)->setState(SensorState::Fast);
            sensorArray.at(14)->setState(SensorState::Medium);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow);
            currentState = State::fireArmed;
            }
            break;
        case command_fire:
            if(currentState == State::fireArmed)
            {
            valveArray.at(0)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(1)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(2)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(3)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(4)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(5)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(6)->setState(ValveState::OpenCommanded);          // Renegade SF Stand
            valveArray.at(7)->setState(ValveState::CloseCommanded);         // Renegade SF Stand
            valveArray.at(8)->setState(ValveState::FireCommanded);          // Renegade SF Stand
            valveArray.at(9)->setState(ValveState::FireCommanded);          // Renegade SF Stand
            pyroArray.at(0)->setState(PyroState::FireCommanded);             // Renegade SF Igniter1
            pyroArray.at(1)->setState(PyroState::FireCommanded);             // Renegade SF Igniter2
/*             pyroArray.at(2)->setState(PyroState::FireCommanded);             // Pasafire Igniter1
            pyroArray.at(3)->setState(PyroState::FireCommanded);             // Pasafire Igniter2 */
            valveEnableArray.at(0)->setState(ValveEnableState::On);
            valveEnableArray.at(1)->setState(ValveEnableState::On);
            valveEnableArray.at(2)->setState(ValveEnableState::Off);
            valveEnableArray.at(3)->setState(ValveEnableState::On);
            valveEnableArray.at(4)->setState(ValveEnableState::On);
            valveEnableArray.at(5)->setState(ValveEnableState::On);
/*             valveEnableArray.at(6)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(7)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(8)->setState(ValveEnableState::Off);        // Pasafire
            valveEnableArray.at(9)->setState(ValveEnableState::Off);        // Pasafire */
            autoSequenceArray.at(0)->setState(AutoSequenceState::RunCommanded);
            sensorArray.at(0)->setState(SensorState::Fast);
            sensorArray.at(1)->setState(SensorState::Fast);
            sensorArray.at(2)->setState(SensorState::Fast);
            sensorArray.at(3)->setState(SensorState::Fast);
            sensorArray.at(4)->setState(SensorState::Fast);
            sensorArray.at(5)->setState(SensorState::Fast);
            sensorArray.at(6)->setState(SensorState::Fast);
            sensorArray.at(7)->setState(SensorState::Fast);
            sensorArray.at(8)->setState(SensorState::Medium);
            sensorArray.at(9)->setState(SensorState::Medium);
            sensorArray.at(10)->setState(SensorState::Medium);
            sensorArray.at(11)->setState(SensorState::Fast);
            sensorArray.at(12)->setState(SensorState::Fast);
            sensorArray.at(13)->setState(SensorState::Fast);
            sensorArray.at(14)->setState(SensorState::Medium);
            sensorArray.at(15)->setState(SensorState::Slow);
            sensorArray.at(16)->setState(SensorState::Slow);
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
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(2)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openLoxVent:
             if(currentState == State::test)
            {
                valveArray.at(2)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(2)->setState(ValveState::OpenCommanded);
            }              
            break;
        case command_closeLoxDomeReg:
            if(currentState == State::test)
            {
                valveArray.at(3)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(3)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openLoxDomeReg:
             if(currentState == State::test)
            {
                valveArray.at(3)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(3)->setState(ValveState::OpenCommanded);
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
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(5)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openFuelVent:
             if(currentState == State::test)
            {
                valveArray.at(5)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(5)->setState(ValveState::OpenCommanded);
            }              
            break;
        case command_closeFuelDomeReg:
            if(currentState == State::test)
            {
                valveArray.at(6)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(6)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openFuelDomeReg:
             if(currentState == State::test)
            {
                valveArray.at(6)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(6)->setState(ValveState::OpenCommanded);
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
        case command_closeLoxMV:
            if(currentState == State::test)
            {
                valveArray.at(8)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(8)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openLoxMV:
             if(currentState == State::test)
            {
                valveArray.at(8)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(8)->setState(ValveState::OpenCommanded);
            }              
            break;
        case command_closeFuelMV:
            if(currentState == State::test)
            {
                valveArray.at(9)->setState(ValveState::CloseCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(9)->setState(ValveState::CloseCommanded);
            }              
            break;
        case command_openFuelMV:
             if(currentState == State::test)
            {
                valveArray.at(9)->setState(ValveState::OpenCommanded);
            }
            else if (currentState == State::offNominal)
            {
                valveArray.at(9)->setState(ValveState::OpenCommanded);
            }              
            break;
        case command_disableHiPressHiVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(0)->setState(ValveEnableState::Off);
                //digitalWrite(pin::HiPressHiVentSafe, 0);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(0)->setState(ValveEnableState::Off);
                //digitalWrite(pin::HiPressHiVentSafe, 0);
            }              
            break;
        case command_enableHiPressHiVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(0)->setState(ValveEnableState::On);
                //digitalWrite(pin::HiPressHiVentSafe, 1);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(0)->setState(ValveEnableState::On);
                //digitalWrite(pin::HiPressHiVentSafe, 1);
            }              
            break;
        case command_disableFuelVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(1)->setState(ValveEnableState::Off);
                //digitalWrite(pin::FuelVentSafe, 0);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(1)->setState(ValveEnableState::Off);
                //digitalWrite(pin::FuelVentSafe, 0);
            }              
            break;
        case command_enableFuelVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(1)->setState(ValveEnableState::On);
                //digitalWrite(pin::FuelVentSafe, 1);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(1)->setState(ValveEnableState::On);
                //digitalWrite(pin::FuelVentSafe, 1);
            }              
            break;
        case command_disableLoxDomeRegLoxDomeVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(2)->setState(ValveEnableState::Off);
                //digitalWrite(pin::LoxDomeRegVentSafe, 0);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(2)->setState(ValveEnableState::Off);
                //digitalWrite(pin::LoxDomeRegVentSafe, 0);
            }              
            break;
        case command_enableLoxDomeRegLoxDomeVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(2)->setState(ValveEnableState::On);
                //digitalWrite(pin::LoxDomeRegVentSafe, 1);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(2)->setState(ValveEnableState::On);
                //digitalWrite(pin::LoxDomeRegVentSafe, 1);
            }              
            break;
        case command_disableFuelDomeRegFuelDomeVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(3)->setState(ValveEnableState::Off);
                //digitalWrite(pin::FuelDomeRegVentSafe, 0);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(3)->setState(ValveEnableState::Off);
                //digitalWrite(pin::FuelDomeRegVentSafe, 0);
            }              
            break;
        case command_enableFuelDomeRegFuelDomeVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(3)->setState(ValveEnableState::On);
                //digitalWrite(pin::FuelDomeRegVentSafe, 1);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(3)->setState(ValveEnableState::On);
                //digitalWrite(pin::FuelDomeRegVentSafe, 1);
            }              
            break;
        case command_disableLoxVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(4)->setState(ValveEnableState::Off);
                //digitalWrite(pin::LoxVentSafe, 0);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(4)->setState(ValveEnableState::Off);
                //digitalWrite(pin::LoxVentSafe, 0);
            }              
            break;
        case command_enableLoxVentSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(4)->setState(ValveEnableState::On);
                //digitalWrite(pin::LoxVentSafe, 1);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(4)->setState(ValveEnableState::On);
                //digitalWrite(pin::LoxVentSafe, 1);
            }              
            break;
        case command_disableMainValvesSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(5)->setState(ValveEnableState::Off);
                //digitalWrite(pin::MainValvesSafe, 0);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(5)->setState(ValveEnableState::Off);
                //digitalWrite(pin::MainValvesSafe, 0);
            }              
            break;
        case command_enableMainValvesSafety:
            if(currentState == State::test)
            {
                valveEnableArray.at(5)->setState(ValveEnableState::On);
                //digitalWrite(pin::MainValvesSafe, 1);
            }
            else if (currentState == State::offNominal)
            {
                valveEnableArray.at(5)->setState(ValveEnableState::On);
                //digitalWrite(pin::MainValvesSafe, 1);
            }              
            break;
        default:
            break;
    }
}