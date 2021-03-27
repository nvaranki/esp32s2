// Sleep and wake-up controller of the MCU.
//
// © 2021 Nikolai Varankine

#include "SleepAndWakeupController.hpp"

SleepAndWakeupController::SleepAndWakeupController() :
    wcGPIO( new BitSetRW( RTC_CNTL_EXT_WAKEUP_CONF_REG, RTC_CNTL_EXT_WAKEUP0_LV_M | RTC_CNTL_EXT_WAKEUP1_LV_M ) ),
    wcEXT1( new BitSetRW( RTC_CNTL_EXT_WAKEUP1_REG, RTC_CNTL_EXT_WAKEUP1_STATUS_CLR_M | RTC_CNTL_EXT_WAKEUP1_SEL_M ) ),
    wsEXT1( new BitSetRO( RTC_CNTL_EXT_WAKEUP1_STATUS_REG, RTC_CNTL_EXT_WAKEUP1_STATUS_M ) ),
    interrupt( new FlagWO( RTC_CNTL_STATE0_REG, RTC_CNTL_SW_CPU_INT_S ) )
{
}

SleepAndWakeupController::~SleepAndWakeupController()
{
    delete wcGPIO;
    delete wcEXT1;
    delete wsEXT1;
    delete interrupt;
}

SleepAndWakeupController::Sleep::Sleep() :
    start( new FlagRW( RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN_S ) )
{
}

SleepAndWakeupController::Sleep::~Sleep()
{
    delete start;
}

SleepAndWakeupController::Reject::Reject() :
    enable( new BitSetRW( RTC_CNTL_SLP_REJECT_CONF_REG, RTC_CNTL_SLEEP_REJECT_ENA_M ) ),
    enableLightSleep( new FlagRW( RTC_CNTL_SLP_REJECT_CONF_REG, RTC_CNTL_LIGHT_SLP_REJECT_EN_S ) ),
    enableDeepSleep( new FlagRW( RTC_CNTL_SLP_REJECT_CONF_REG, RTC_CNTL_DEEP_SLP_REJECT_EN_S ) ),
    on( new FlagRW( RTC_CNTL_STATE0_REG, RTC_CNTL_SLP_REJECT_S ) ),
    cause( new BitSetRO( RTC_CNTL_SLP_REJECT_CAUSE_REG, RTC_CNTL_REJECT_CAUSE_M ) ),
    clear( new FlagWO( RTC_CNTL_STATE0_REG, RTC_CNTL_SLP_REJECT_CAUSE_CLR_S ) )
{
}

SleepAndWakeupController::Reject::~Reject()
{
    delete enable;
    delete enableLightSleep;
    delete enableDeepSleep;
    delete on;
    delete cause;
    delete clear;
}

SleepAndWakeupController::Wakeup::Wakeup() :
    enable( new BitSetRW( RTC_CNTL_WAKEUP_STATE_REG, RTC_CNTL_WAKEUP_ENA_M ) ),
    on( new FlagRW( RTC_CNTL_STATE0_REG, RTC_CNTL_SLP_WAKEUP_S ) ),
    cause( new BitSetRO( RTC_CNTL_SLP_WAKEUP_CAUSE_REG, RTC_CNTL_WAKEUP_CAUSE_M ) )
{
}

SleepAndWakeupController::Wakeup::~Wakeup()
{
    delete enable;
    delete on;
    delete cause;
}
