// Low power fast clocks controller for RTC power domains.
// It feeds ULP-coprocessor, Sensor controllers, RTC slow memory and RTC registers.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "FastClockRTC.hpp"

FastClockRTC::FastClockRTC() :
    src( new BitSetRW( RTC_CNTL_CLK_CONF_REG, RTC_CNTL_FAST_CLK_RTC_SEL_M ) )
{
}

FastClockRTC::~FastClockRTC() 
{
    delete src;    
}

FastClockRTC::Source FastClockRTC::getSource() const 
{ 
    return (Source) src->get( RTC_CNTL_FAST_CLK_RTC_SEL_M ); 
}

void FastClockRTC::setSource( const FastClockRTC::Source value ) 
{ 
    src->set( RTC_CNTL_FAST_CLK_RTC_SEL_M, static_cast<bool>( value ) ); 
}
