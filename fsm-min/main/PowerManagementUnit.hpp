// RTC_CNTL

#ifndef H_PowerManagementUnit
#define H_PowerManagementUnit

#include "SleepAndWakeupController.hpp"

class PowerManagementUnit
{
private:
    SleepAndWakeupController* sw_ctrl;
public:
    PowerManagementUnit(/* args */);
    virtual ~PowerManagementUnit();
public:
    SleepAndWakeupController* getSleepAndWakeupController();
};

#endif