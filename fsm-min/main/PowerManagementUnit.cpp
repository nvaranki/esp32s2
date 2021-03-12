// Power management unit of the MCU.
//
// © 2021 Nikolai Varankine

#include "PowerManagementUnit.hpp"

PowerManagementUnit::PowerManagementUnit() :
    swc( nullptr )
{
}

PowerManagementUnit::~PowerManagementUnit()
{
    if( swc != nullptr ) delete swc;
}

SleepAndWakeupController* PowerManagementUnit::getSleepAndWakeupController()
{
    if( swc == nullptr )
        swc = new SleepAndWakeupController();
    return swc;
}
