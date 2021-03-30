// Power management unit of the MCU.
//
// © 2021 Nikolai Varankine

#include "soc/apb_ctrl_reg.h"
#include "soc/bb_reg.h"
#include "soc/fe_reg.h"
#include "soc/i2s_reg.h"
#include "soc/nrx_reg.h"
#include "PowerManagementUnit.hpp"

PowerManagementUnit::PowerManagementUnit() :
    swc( nullptr ),
    timer( nullptr ),
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
    if( timer != nullptr ) delete timer;
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

PowerManagementUnit::Switch::WiFiRX::WiFiRX() :
    powerEST( new Trigger2(
        new FlagRW( NRXPD_CTRL, NRX_CHAN_EST_FORCE_PU_S ),
        new FlagRW( NRXPD_CTRL, NRX_CHAN_EST_FORCE_PD_S ) ) ),
    powerROT( new Trigger2(
        new FlagRW( NRXPD_CTRL, NRX_RX_ROT_FORCE_PU_S ),
        new FlagRW( NRXPD_CTRL, NRX_RX_ROT_FORCE_PD_S ) ) ),
    powerVIT( new Trigger2(
        new FlagRW( NRXPD_CTRL, NRX_VIT_FORCE_PU_S ),
        new FlagRW( NRXPD_CTRL, NRX_VIT_FORCE_PD_S ) ) ),
    powerDEMAP( new Trigger2(
        new FlagRW( NRXPD_CTRL, NRX_DEMAP_FORCE_PU_S ),
        new FlagRW( NRXPD_CTRL, NRX_DEMAP_FORCE_PD_S ) ) )
{
}

PowerManagementUnit::Switch::WiFiRX::~WiFiRX()
{
    delete powerEST->on;
    delete powerEST->off;
    delete powerEST;
    delete powerROT->on;
    delete powerROT->off;
    delete powerROT;
    delete powerVIT->on;
    delete powerVIT->off;
    delete powerVIT;
    delete powerDEMAP->on;
    delete powerDEMAP->off;
    delete powerDEMAP;
}

PowerManagementUnit::Switch::WiFiRF::WiFiRF() :
    powerIQ( new Trigger2(
        new FlagRW( FE_GEN_CTRL, FE_IQ_EST_FORCE_PU_S ),
        new FlagRW( FE_GEN_CTRL, FE_IQ_EST_FORCE_PD_S ) ) ),
    powerTX( new Trigger2(
        new FlagRW( FE2_TX_INTERP_CTRL, FE2_TX_INF_FORCE_PU_S ),
        new FlagRW( FE2_TX_INTERP_CTRL, FE2_TX_INF_FORCE_PD_S ) ) )
{
}

PowerManagementUnit::Switch::WiFiRF::~WiFiRF()
{
    delete powerIQ->on;
    delete powerIQ->off;
    delete powerIQ;
    delete powerTX->on;
    delete powerTX->off;
    delete powerTX;
}

PowerManagementUnit::Switch::SerialI2S::SerialI2S() :
    clockDMA  ( new FlagRW( I2S_PD_CONF_REG(0), I2S_DMA_RAM_CLK_FO_S ) ),
    powerDMA( new Trigger2(
        new FlagRW( I2S_PD_CONF_REG(0), I2S_DMA_RAM_FORCE_PU_S ),
        new FlagRW( I2S_PD_CONF_REG(0), I2S_DMA_RAM_FORCE_PD_S ) ) ),
    powerPLC( new Trigger2(
        new FlagRW( I2S_PD_CONF_REG(0), I2S_PLC_MEM_FORCE_PU_S ),
        new FlagRW( I2S_PD_CONF_REG(0), I2S_DMA_RAM_FORCE_PD_S ) ) ),
    powerFIFO( new Trigger2(
        new FlagRW( I2S_PD_CONF_REG(0), I2S_FIFO_FORCE_PU_S ),
        new FlagRW( I2S_PD_CONF_REG(0), I2S_FIFO_FORCE_PD_S ) ) )
{
}

PowerManagementUnit::Switch::SerialI2S::~SerialI2S()
{
    delete clockDMA;
    delete powerDMA->on;
    delete powerDMA->off;
    delete powerDMA;
    delete powerPLC->on;
    delete powerPLC->off;
    delete powerPLC;
    delete powerFIFO->on;
    delete powerFIFO->off;
    delete powerFIFO;
}

PowerManagementUnit::Switch::BusAPB::BusAPB() :
    powerDC( new Trigger2(
        new FlagRW( APB_CTRL_FRONT_END_MEM_PD_REG, APB_CTRL_DC_MEM_FORCE_PU_S ),
        new FlagRW( APB_CTRL_FRONT_END_MEM_PD_REG, APB_CTRL_DC_MEM_FORCE_PD_S ) ) ),
    powerPBUS( new Trigger2(
        new FlagRW( APB_CTRL_FRONT_END_MEM_PD_REG, APB_CTRL_PBUS_MEM_FORCE_PU_S ),
        new FlagRW( APB_CTRL_FRONT_END_MEM_PD_REG, APB_CTRL_PBUS_MEM_FORCE_PD_S ) ) ),
    powerAGC( new Trigger2(
        new FlagRW( APB_CTRL_FRONT_END_MEM_PD_REG, APB_CTRL_AGC_MEM_FORCE_PU_S ),
        new FlagRW( APB_CTRL_FRONT_END_MEM_PD_REG, APB_CTRL_AGC_MEM_FORCE_PD_S ) ) )
{
}

PowerManagementUnit::Switch::BusAPB::~BusAPB()
{
    delete powerDC->on;
    delete powerDC->off;
    delete powerDC;
    delete powerPBUS->on;
    delete powerPBUS->off;
    delete powerPBUS;
    delete powerAGC->on;
    delete powerAGC->off;
    delete powerAGC;
}

PowerManagementUnit::Switch::BaseBand::BaseBand() :
    powerFFT( new Trigger2(
        new FlagRW( BBPD_CTRL, BB_FFT_FORCE_PU_S ),
        new FlagRW( BBPD_CTRL, BB_FFT_FORCE_PD_S ) ) ),
    powerDC( new Trigger2(
        new FlagRW( BBPD_CTRL, BB_DC_EST_FORCE_PU_S ),
        new FlagRW( BBPD_CTRL, BB_DC_EST_FORCE_PD_S ) ) )
{
}

PowerManagementUnit::Switch::BaseBand::~BaseBand()
{
    delete powerFFT->on;
    delete powerFFT->off;
    delete powerFFT;
    delete powerDC->on;
    delete powerDC->off;
    delete powerDC;
}

TimerRTC* PowerManagementUnit::getTimer()
{
    if( timer == nullptr ) timer = new TimerRTC();
    return timer;
}
