// Timer of ULP co-processor.
//
// © 2021 Nikolai Varankine

#ifndef H_TimerULP
#define H_TimerULP

#include "esp_bit_defs.h"
#include "soc/rtc_cntl_reg.h"
#include "bits/BitSetRW.hpp"
#include "bits/SubValueRW.hpp"

class TimerULP
{
private:
    BitSetRW* const cfg; //!< configuration register
    SubValueRW* const sleep; //!< sleep cycles for ULP coprocessor timer
public:
    TimerULP();
    virtual ~TimerULP();
public:
    enum class Configuration : uint32_t
    {
        // RTC_CNTL_ULP_CP_TIMER_REG (0x00F8)
        // see missing bits and values in separate registers
        TIMER  = RTC_CNTL_ULP_CP_SLP_TIMER_EN, //!< ULP co-processor timer enable, 0: Disable hardware timer; 1: Enable hardware timer
    };
public:
    bool getConfig( const Configuration test ) const { return cfg->get( static_cast<uint32_t>( test ) ); };
    void setConfig( const Configuration mask, bool value ) { cfg->set( static_cast<uint32_t>( mask ), value ); };
    void setSleepCycles( uint32_t value ) { sleep->set( value ); }
    uint32_t getSleepCycles() { return sleep->get(); }
};

#endif