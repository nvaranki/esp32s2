// Low power slow clocks controller for RTC power domains.
//
// © 2021 Nikolai Varankine

#ifndef H_SlowClockRTC
#define H_SlowClockRTC

#include <stdint.h>
#include "bits/SubValueRW.hpp"

class SlowClockRTC
{
private:
    SubValueRW* const src;
public:
    SlowClockRTC();
    virtual ~SlowClockRTC();
public:
    enum class Source : uint32_t //!< RTC slow clock
    {
        //  RTC_CNTL_CLK_CONF_REG (0x0074) RTC_CNTL_ANA_CLK_RTC_SEL
        RTC90K   = 0, //!< 90K rtc_clk
        XTAL32K  = 1, //!< 32k XTAL
        CK8MD256 = 2, //!< 8md256
    };
public:
    Source getSource() const;
    void setSource( const Source value );
};

#endif
