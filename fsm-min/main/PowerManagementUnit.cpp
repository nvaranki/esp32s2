#include "PowerManagementUnit.hpp"

SleepAndWakeupController* PowerManagementUnit::getSleepAndWakeupController()
{
    if( sw_ctrl == nullptr )
        sw_ctrl = new SleepAndWakeupController();
    return sw_ctrl;
}

PowerManagementUnit::PowerManagementUnit(/* args */)
{
}

PowerManagementUnit::~PowerManagementUnit()
{
    if( sw_ctrl != nullptr )
        delete sw_ctrl;
}
