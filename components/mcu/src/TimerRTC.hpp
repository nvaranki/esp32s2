// Timer of RTC. It is always ON.
//
// © 2021 Nikolai Varankine

#ifndef H_TimerRTC
#define H_TimerRTC

#include "esp_bit_defs.h"
#include "esp_err.h"
#include "soc/rtc_cntl_reg.h"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRW.hpp"

class TimerRTC
{
private:
    /** Sets the lower 32 bits of the trigger threshold */
    SubValueRW* const ttl;
    /** Sets the higher 16 bits of the trigger threshold */
    SubValueRW* const tth;
    /** Sets the lower 32 bits of the trigger threshold */
    SubValueRW* const t0l;
    /** Sets the higher 16 bits of the trigger threshold */
    SubValueRW* const t0h;
    /** Sets the lower 32 bits of the trigger threshold */
    SubValueRW* const t1l;
    /** Sets the higher 16 bits of the trigger threshold */
    SubValueRW* const t1h;
public:
    /** Sets this bit to update and backup current timestamp */
    FlagWO* const update;
    /** Sets this bit to enable the timer alarm */
    FlagWO* const alarm;
    /** enable RTC main timer interrupt */
    FlagRW* const interrupt;
    FlagWO* const clearInterrupt;
public:
    TimerRTC();
    virtual ~TimerRTC();
public:
    esp_err_t setTimeStampAlarm( uint64_t t );
    uint64_t getTimeStampCurrent() const;
    uint64_t getTimeStampBackup() const;
};

#endif
