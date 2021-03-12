// Timer of ULP co-processor.
//
// © 2021 Nikolai Varankine

#include "TimerULP.hpp"

#include "soc/rtc_cntl_reg.h"
#include "CoreRISCV.hpp"

TimerULP::TimerULP() :
    cfg( new BitSetRW( RTC_CNTL_ULP_CP_TIMER_REG, 0x06402001 ) ), //TODO predefined constants
    sleep(  new SubValueRW( RTC_CNTL_ULP_CP_TIMER_1_REG, 0xFFFFFFF0, 8 ) ) //TODO predefined constants
{
}

TimerULP::~TimerULP()
{
    delete cfg;
    delete sleep;
}