// RTC_CNTL

// Power management unit of the MCU.
//
// © 2021 Nikolai Varankine

#ifndef H_PowerManagementUnit
#define H_PowerManagementUnit

#include "SleepAndWakeupController.hpp"

class PowerManagementUnit
{
private:
    SleepAndWakeupController* swc;
public:
    PowerManagementUnit();
    virtual ~PowerManagementUnit();
public:
    SleepAndWakeupController* getSleepAndWakeupController();
};

#endif