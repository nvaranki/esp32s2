// Timer of RTC. It is always ON.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "TimerRTC.hpp"

TimerRTC::TimerRTC() :
    ttl( new SubValueRW( RTC_CNTL_SLP_TIMER0_REG, RTC_CNTL_SLP_VAL_LO_M, RTC_CNTL_SLP_VAL_LO_S ) ),
    tth( new SubValueRW( RTC_CNTL_SLP_TIMER1_REG, RTC_CNTL_SLP_VAL_HI_M, RTC_CNTL_SLP_VAL_HI_S ) ),
    t0l( new SubValueRW( RTC_CNTL_TIME_LOW0_REG,  RTC_CNTL_TIMER_VALUE0_LOW_M,  RTC_CNTL_TIMER_VALUE0_LOW_S ) ),
    t0h( new SubValueRW( RTC_CNTL_TIME_HIGH0_REG, RTC_CNTL_TIMER_VALUE0_HIGH_M, RTC_CNTL_TIMER_VALUE0_HIGH_S ) ),
    t1l( new SubValueRW( RTC_CNTL_TIME_LOW1_REG,  RTC_CNTL_TIMER_VALUE1_LOW_M,  RTC_CNTL_TIMER_VALUE1_LOW_S ) ),
    t1h( new SubValueRW( RTC_CNTL_TIME_HIGH1_REG, RTC_CNTL_TIMER_VALUE1_HIGH_M, RTC_CNTL_TIMER_VALUE1_HIGH_S ) ),
    update( new FlagWO( RTC_CNTL_TIME_UPDATE_REG, RTC_CNTL_TIME_UPDATE_S ) ),
    alarm( new FlagWO( RTC_CNTL_SLP_TIMER1_REG, RTC_CNTL_MAIN_TIMER_ALARM_EN_S ) ),
    interrupt( new FlagRW( RTC_CNTL_INT_ENA_REG, RTC_CNTL_MAIN_TIMER_INT_ENA_S ) ),
    clearInterrupt( new FlagWO( RTC_CNTL_INT_CLR_REG, RTC_CNTL_MAIN_TIMER_INT_CLR_S ) )
{
}

TimerRTC::~TimerRTC()
{
    delete ttl;
    delete tth;
    delete t0l;
    delete t0h;
    delete t1l;
    delete t1h;
    delete alarm;
    delete interrupt;
    delete clearInterrupt;
}

#define RTC_CNTL_TIMER_VALUE0_LOW_W 32
#define RTC_CNTL_TIMER_VALUE1_LOW_W 32
#define RTC_CNTL_SLP_VAL_LO_W 32

esp_err_t TimerRTC::setTimeStampAlarm( uint64_t t )
{
    if( ( t >> RTC_CNTL_SLP_VAL_LO_W ) & ~RTC_CNTL_SLP_VAL_HI_V ) return ESP_ERR_INVALID_ARG;

    ttl->set( (uint32_t) ( t & RTC_CNTL_SLP_VAL_LO_V ) );
    tth->set( (uint32_t) ( ( t >> RTC_CNTL_SLP_VAL_LO_W ) & RTC_CNTL_SLP_VAL_HI_V ) );

    return ESP_OK;
}

uint64_t TimerRTC::getTimeStampCurrent() const
{
    return (uint64_t) t0h->get() << RTC_CNTL_TIMER_VALUE0_LOW_W | t0l->get();
}

uint64_t TimerRTC::getTimeStampBackup() const
{
    return (uint64_t) t1h->get() << RTC_CNTL_TIMER_VALUE1_LOW_W | t1l->get();
}

