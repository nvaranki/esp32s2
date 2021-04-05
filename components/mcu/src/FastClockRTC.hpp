// Low power fast clocks controller for RTC power domains.
// It feeds ULP-coprocessor, Sensor controllers, RTC slow memory and RTC registers.
//
// © 2021 Nikolai Varankine

#ifndef H_FastClockRTC
#define H_FastClockRTC

#include "bits/BitSetRW.hpp"

class FastClockRTC
{
private:
    BitSetRW* const src;
public:
    FastClockRTC();
    virtual ~FastClockRTC();
public:
    enum struct Source : bool
    {
        // RTC_CNTL_CLK_CONF_REG (0x0074) RTC_CNTL_FAST_CLK_RTC_SEL
        XTAL = 0, //!< XTAL_CLK div 4
        CK8M = 1, //!< RTC8M_CLK div N
    };
public:
    Source getSource() const;
    void setSource( const Source value );
};

#endif
