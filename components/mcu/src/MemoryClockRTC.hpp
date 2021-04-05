// Low power clocks controller for RTC slow and fast memory.
//
// © 2021 Nikolai Varankine

#ifndef H_MemoryClockRTC
#define H_MemoryClockRTC

#include <stdint.h>
#include "soc/system_reg.h"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class MemoryClockRTC
{
private:
    SubValueRW* const src;
public:
    FlagRW* const enable;
    SubValueRW* const divider;  //!< post-mux divider by (divider+1)? TODO undocumented
    SubValueRW* const dividerA; //TODO undocumented
    SubValueRW* const dividerB; //TODO undocumented
public:
    MemoryClockRTC();
    virtual ~MemoryClockRTC();
public:
    enum class Source : uint32_t //!< RTC slow/fast memory clock
    {
        // DPORT_BT_LPCK_DIV_FRAC_REG (0x0054)
        RTC90K  = DPORT_LPCLK_SEL_RTC_SLOW_M, //!< 90k RTC slow clock
        CK8M    = DPORT_LPCLK_SEL_8M_M, //!< 8M clock
        XTAL    = DPORT_LPCLK_SEL_XTAL_M, //!< 40M XTAL clock
        XTAL32K = DPORT_LPCLK_SEL_XTAL32K_M, //!< 32k XTAL clock
    };
public:
    Source getSource() const;
    void setSource( const Source value );
};

#endif
