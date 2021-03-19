// Timer of ULP co-processor.
//
// © 2021 Nikolai Varankine

#ifndef H_TimerULP
#define H_TimerULP

#include "esp_bit_defs.h"
#include "soc/rtc_cntl_reg.h"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class TimerULP
{
public:
    /**
     * Contains work status of the timer.
     * When set to true (active), the timer starts counting immediately. After counter expires, timer signals 
     * current ULP processor to wake up and start their program to run. After processor ends and 
     * shuts down, the timer repeats counting again with fresh counter but only if the processor or
     * GPIO is assigned to restart timer.
     * When set to false (passive), the timer just ignores next attempts to (re)start it by other means.
     */
    FlagRW* const active;
    /**
     * Contains initial number of counter's sleep cycles.
     */
    SubValueRW* const sleep;
public:
    TimerULP();
    virtual ~TimerULP();
};

#endif
