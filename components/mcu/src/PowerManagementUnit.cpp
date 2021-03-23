// Power management unit of the MCU.
//
// © 2021 Nikolai Varankine

#include "PowerManagementUnit.hpp"

PowerManagementUnit::PowerManagementUnit() :
    swc( nullptr ),
    status( new SubValueRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_M, RTC_CNTL_MAIN_STATE_S) ),
    idle( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_IN_IDLE_S ) ),
    sleep( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_IN_SLP_S ) ),
    waitXTAL( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_IN_WAIT_XTL_S ) ),
    waitPLL( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_IN_WAIT_PLL_S ) ),
    waitO8M( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_IN_WAIT_8M_S ) ),
    standby( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_IN_LOW_POWER_STATE_S ) ),
    wakeup( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_IN_WAKEUP_STATE_S ) ),
    wait( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_WAIT_END_S ) ),
    ready( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_RDY_FOR_WAKEUP_S ) ),
    pll( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_PLL_ON_S ) ),
    xtalISO( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_MAIN_STATE_XTAL_ISO_S ) ),
    cocpuDone( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_COCPU_STATE_DONE_S ) ),
    cocpuSleep( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_COCPU_STATE_SLP_S ) ),
    cocpuWork( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_COCPU_STATE_SWITCH_S ) ),
    cocpuRuns( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_COCPU_STATE_START_S ) ),
    touchDone( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_TOUCH_STATE_DONE_S ) ),
    touchSleep( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_TOUCH_STATE_SLP_S ) ),
    touchWork( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_TOUCH_STATE_SWITCH_S ) ),
    touchStart( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_TOUCH_STATE_START_S ) ),
    digiOff( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_XPD_DIG_S ) ),
    digiISO( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_DIG_ISO_S ) ),
    wifiOff( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_XPD_WIFI_S ) ),
    wifiISO( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_WIFI_ISO_S ) ),
    periOff( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_XPD_RTC_PERI_S ) ),
    periISO( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_PERI_ISO_S ) ),
    dcdcOff( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_XPD_DIG_DCDC_S ) ),
    rom0Off( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_XPD_ROM0_S ) )
{
}

PowerManagementUnit::~PowerManagementUnit()
{
    if( swc  != nullptr ) delete swc;
    delete status;
    delete idle;
    delete sleep;
    delete waitXTAL;
    delete waitPLL;
    delete waitO8M;
    delete standby;
    delete wakeup;
    delete wait;
    delete ready;
    delete pll;
    delete xtalISO;
    delete cocpuDone;
    delete cocpuSleep;
    delete cocpuWork;
    delete cocpuRuns;
    delete touchDone;
    delete touchSleep;
    delete touchWork;
    delete touchStart;
    delete digiOff;
    delete digiISO;
    delete wifiOff;
    delete wifiISO;
    delete periOff;
    delete periISO;
    delete dcdcOff;
    delete rom0Off;
}

SleepAndWakeupController* PowerManagementUnit::getSleepAndWakeupController()
{
    if( swc == nullptr )
        swc = new SleepAndWakeupController();
    return swc;
}
