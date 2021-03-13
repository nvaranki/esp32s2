// Sleep and wake-up controller of the MCU.
//
// © 2021 Nikolai Varankine

#include "SleepAndWakeupController.hpp"

SleepAndWakeupController::SleepAndWakeupController() :
    cmd( new BitSetWO( RTC_CNTL_STATE0_REG, 0x00000003 ) ), //TODO mask
    sts( new BitSetRW( RTC_CNTL_STATE0_REG, 0xE0000000 ) ), //TODO mask
    we( new BitSetRW( RTC_CNTL_WAKEUP_STATE_REG, RTC_CNTL_WAKEUP_ENA_M ) ),
    wcGPIO( new BitSetRW( RTC_CNTL_EXT_WAKEUP_CONF_REG, RTC_CNTL_EXT_WAKEUP0_LV_M | RTC_CNTL_EXT_WAKEUP1_LV_M ) ),
    sro( new BitSetRW( RTC_CNTL_SLP_REJECT_CONF_REG, RTC_CNTL_DEEP_SLP_REJECT_EN_M | RTC_CNTL_LIGHT_SLP_REJECT_EN_M | RTC_CNTL_SLEEP_REJECT_ENA_M ) ),
    wcEXT1( new BitSetRW( RTC_CNTL_EXT_WAKEUP1_REG, RTC_CNTL_EXT_WAKEUP1_STATUS_CLR_M | RTC_CNTL_EXT_WAKEUP1_SEL_M ) ),
    wsEXT1( new BitSetRO( RTC_CNTL_EXT_WAKEUP1_STATUS_REG, RTC_CNTL_EXT_WAKEUP1_STATUS_M ) ),
    r2sc( new BitSetRO( RTC_CNTL_SLP_REJECT_CAUSE_REG, RTC_CNTL_REJECT_CAUSE_M ) ),
    s2wc( new BitSetRO( RTC_CNTL_SLP_WAKEUP_CAUSE_REG, RTC_CNTL_WAKEUP_CAUSE_M ) )
{
}

SleepAndWakeupController::~SleepAndWakeupController()
{
    delete cmd;
    delete sts;
    delete we;
    delete wcGPIO;
    delete sro;
    delete wcEXT1;
    delete wsEXT1;
    delete r2sc;
    delete s2wc;
}
