// Timer of ULP co-processor.
//
// © 2021 Nikolai Varankine

#include "TimerULP.hpp"

#include "soc/rtc_cntl_reg.h"
#include "CoreRISCV.hpp"

TimerULP::TimerULP() :
    active( new FlagRW( RTC_CNTL_ULP_CP_TIMER_REG, RTC_CNTL_ULP_CP_SLP_TIMER_EN_S ) ),
    sleep(  new SubValueRW( RTC_CNTL_ULP_CP_TIMER_1_REG, RTC_CNTL_ULP_CP_TIMER_SLP_CYCLE_M, 
        RTC_CNTL_ULP_CP_TIMER_SLP_CYCLE_S ) )
{
}

TimerULP::~TimerULP()
{
    delete active;
    delete sleep;
}