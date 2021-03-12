// Sleep and wake-up controller of the MCU.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "SleepAndWakeupController.hpp"

SleepAndWakeupController::SleepAndWakeupController() :
    we( new RegisterRW( RTC_CNTL_WAKEUP_STATE_REG, RTC_CNTL_WAKEUP_ENA_M ) ),
    wcGPIO( new RegisterRW( RTC_CNTL_EXT_WAKEUP_CONF_REG, RTC_CNTL_EXT_WAKEUP0_LV_M | RTC_CNTL_EXT_WAKEUP1_LV_M ) ),
    sro( new RegisterRW( RTC_CNTL_SLP_REJECT_CONF_REG, RTC_CNTL_DEEP_SLP_REJECT_EN_M | RTC_CNTL_LIGHT_SLP_REJECT_EN_M | RTC_CNTL_SLEEP_REJECT_ENA_M ) ),
    wcEXT1( new RegisterRW( RTC_CNTL_EXT_WAKEUP1_REG, RTC_CNTL_EXT_WAKEUP1_STATUS_CLR_M | RTC_CNTL_EXT_WAKEUP1_SEL_M ) ),
    wsEXT1( new RegisterRO( RTC_CNTL_EXT_WAKEUP1_STATUS_REG, RTC_CNTL_EXT_WAKEUP1_STATUS_M ) ),
    r2sc( new RegisterRO( RTC_CNTL_SLP_REJECT_CAUSE_REG, RTC_CNTL_REJECT_CAUSE_M ) ),
    s2wc( new RegisterRO( RTC_CNTL_SLP_WAKEUP_CAUSE_REG, RTC_CNTL_WAKEUP_CAUSE_M ) )
{
}

SleepAndWakeupController::~SleepAndWakeupController()
{
    delete we;
    delete wcGPIO;
    delete sro;
    delete wcEXT1;
    delete wsEXT1;
    delete r2sc;
    delete s2wc;
}
