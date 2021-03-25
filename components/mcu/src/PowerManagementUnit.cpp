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
    delete dcdcOff;
    delete rom0Off;
}

SleepAndWakeupController* PowerManagementUnit::getSleepAndWakeupController()
{
    if( swc == nullptr )
        swc = new SleepAndWakeupController();
    return swc;
}

PowerManagementUnit::Regulator::Rtc::Rtc() :
    powerUp( new FlagRW( RTC_CNTL_REG, RTC_CNTL_REGULATOR_FORCE_PU_S ) ),
    powerDn( new FlagRW( RTC_CNTL_REG, RTC_CNTL_REGULATOR_FORCE_PD_S ) ),
    biasSleep ( new SubValueRW( RTC_CNTL_REG, RTC_CNTL_DBIAS_WAK_M, RTC_CNTL_DBIAS_WAK_S ) ),
    biasWakeup( new SubValueRW( RTC_CNTL_REG, RTC_CNTL_DBIAS_SLP_M, RTC_CNTL_DBIAS_SLP_S ) )
{
}

PowerManagementUnit::Regulator::Rtc::~Rtc()
{
    delete powerUp;
    delete powerDn;
    delete biasSleep;
    delete biasWakeup;
}

PowerManagementUnit::Regulator::Digital::Digital() :
//  powerUp( new FlagRW( RTC_CNTL_REG, RTC_CNTL_REGULATOR_FORCE_PU_S ) ), //TODO 29?
//  powerDn( new FlagRW( RTC_CNTL_REG, RTC_CNTL_REGULATOR_FORCE_PD_S ) ), //TODO 28?
    biasSleep ( new SubValueRW( RTC_CNTL_REG, RTC_CNTL_DIG_DBIAS_WAK_M, RTC_CNTL_DIG_DBIAS_WAK_S ) ),
    biasWakeup( new SubValueRW( RTC_CNTL_REG, RTC_CNTL_DIG_DBIAS_SLP_M, RTC_CNTL_DIG_DBIAS_SLP_S ) )
{
}

PowerManagementUnit::Regulator::Digital::~Digital()
{
//  delete powerUp;
//  delete powerDn;
    delete biasSleep;
    delete biasWakeup;
}

PowerManagementUnit::Switch::Peripherals::Peripherals() :
    isolation( new Trigger3(
        new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_PERI_ISO_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FORCE_ISO_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FORCE_NOISO_S ) ) ),
    power( new Trigger3(
        new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_XPD_RTC_PERI_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FORCE_PU_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FORCE_PD_S ) ) ),
    sleepDn ( new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_PD_EN_S ) ),
    holdGPIO( new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_PAD_FORCE_HOLD_S ) )
{
}

PowerManagementUnit::Switch::Peripherals::~Peripherals()
{
    delete isolation->state;
    delete isolation->on;
    delete isolation->off;
    delete isolation;
    delete power->state;
    delete power->on;
    delete power->off;
    delete power;
    delete sleepDn;
    delete holdGPIO;
}

PowerManagementUnit::Switch::FastMemory::FastMemory() :
    isolation( new Trigger2(
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FASTMEM_FORCE_ISO_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FASTMEM_FORCE_NOISO_S ) ) ),
    power( new Trigger2(
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FASTMEM_FORCE_PU_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FASTMEM_FORCE_PD_S ) ) ),
    sleepDn( new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FASTMEM_PD_EN_S ) ),
    retain( new Trigger2(
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FASTMEM_FORCE_LPU_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FASTMEM_FORCE_LPD_S ) ) ),
    followCPU( new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_FASTMEM_FOLW_CPU_S ) )
{
}

PowerManagementUnit::Switch::FastMemory::~FastMemory()
{
    delete isolation->on;
    delete isolation->off;
    delete isolation;
    delete power->on;
    delete power->off;
    delete power;
    delete sleepDn;
    delete retain->on;
    delete retain->off;
    delete retain;
    delete followCPU;
}

PowerManagementUnit::Switch::SlowMemory::SlowMemory() :
    isolation( new Trigger2(
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_SLOWMEM_FORCE_ISO_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_SLOWMEM_FORCE_NOISO_S ) ) ),
    power( new Trigger2(
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_SLOWMEM_FORCE_PU_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_SLOWMEM_FORCE_PD_S ) ) ),
    sleepDn( new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_SLOWMEM_PD_EN_S ) ),
    retain( new Trigger2(
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_SLOWMEM_FORCE_LPU_S ),
        new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_SLOWMEM_FORCE_LPD_S ) ) ),
    followCPU( new FlagRW( RTC_CNTL_PWC_REG, RTC_CNTL_SLOWMEM_FOLW_CPU_S ) )
{
}

PowerManagementUnit::Switch::SlowMemory::~SlowMemory()
{
    delete isolation->on;
    delete isolation->off;
    delete isolation;
    delete power->on;
    delete power->off;
    delete power;
    delete sleepDn;
    delete retain->on;
    delete retain->off;
    delete retain;
    delete followCPU;
}

PowerManagementUnit::Switch::Digital::Digital() :
    isolation( new Trigger3(
        new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_DIG_ISO_S ),
        new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_DG_WRAP_FORCE_ISO_S ),
        new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_DG_WRAP_FORCE_NOISO_S ) ) ),
    power( new Trigger3(
        new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_XPD_DIG_S ),
        new FlagRW( RTC_CNTL_DIG_PWC_REG, RTC_CNTL_DG_WRAP_FORCE_PU_S ),
        new FlagRW( RTC_CNTL_DIG_PWC_REG, RTC_CNTL_DG_WRAP_FORCE_PD_S ) ) ),
    sleepDn( new FlagRW( RTC_CNTL_DIG_PWC_REG, RTC_CNTL_DG_WRAP_PD_EN_S ) ),
    memory( new Trigger2(
        new FlagRW( RTC_CNTL_DIG_PWC_REG, RTC_CNTL_LSLP_MEM_FORCE_PU_S ),
        new FlagRW( RTC_CNTL_DIG_PWC_REG, RTC_CNTL_LSLP_MEM_FORCE_PD_S ) ) )
{
}

PowerManagementUnit::Switch::Digital::~Digital()
{
    delete isolation->state;
    delete isolation->on;
    delete isolation->off;
    delete isolation;
    delete power->state;
    delete power->on;
    delete power->off;
    delete power;
    delete sleepDn;
    delete memory->on;
    delete memory->off;
    delete memory;
}

PowerManagementUnit::Switch::DigitalGPIO::DigitalGPIO() :
    isolation( new Trigger2(
        new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_DG_PAD_FORCE_ISO_S ),
        new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_DG_PAD_FORCE_NOISO_S ) ) ),
    hold( new Trigger2(
        new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_DG_PAD_FORCE_HOLD_S ),
        new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_DG_PAD_FORCE_UNHOLD_S ) ) ),
    holdAutoOff ( new FlagWO( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_CLR_DG_PAD_AUTOHOLD_S ) ),
    holdAutoOn  ( new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_DG_PAD_AUTOHOLD_EN_S ) ),
    holdAuto    ( new FlagRO( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_DG_PAD_AUTOHOLD_S ) )
{
}

PowerManagementUnit::Switch::DigitalGPIO::~DigitalGPIO()
{
    delete isolation->on;
    delete isolation->off;
    delete isolation;
    delete hold->on;
    delete hold->off;
    delete hold;
    delete holdAutoOff;
    delete holdAutoOn;
    delete holdAuto;
}

PowerManagementUnit::Switch::WiFi::WiFi() :
    isolation( new Trigger3(
        new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_WIFI_ISO_S ),
        new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_WIFI_FORCE_ISO_S ),
        new FlagRW( RTC_CNTL_DIG_ISO_REG, RTC_CNTL_WIFI_FORCE_NOISO_S ) ) ),
    power( new Trigger3(
        new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_XPD_WIFI_S ),
        new FlagRW( RTC_CNTL_DIG_PWC_REG, RTC_CNTL_WIFI_FORCE_PU_S ),
        new FlagRW( RTC_CNTL_DIG_PWC_REG, RTC_CNTL_WIFI_FORCE_PD_S ) ) ),
    sleepDn( new FlagRW( RTC_CNTL_DIG_PWC_REG, RTC_CNTL_WIFI_PD_EN_S ) )
{
}

PowerManagementUnit::Switch::WiFi::~WiFi()
{
    delete isolation->state;
    delete isolation->on;
    delete isolation->off;
    delete isolation;
    delete power->state;
    delete power->on;
    delete power->off;
    delete power;
    delete sleepDn;
}
