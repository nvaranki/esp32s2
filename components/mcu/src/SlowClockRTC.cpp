// Low power slow clocks controller for RTC power domains.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "SlowClockRTC.hpp"

SlowClockRTC::SlowClockRTC() :
    src( new SubValueRW( RTC_CNTL_CLK_CONF_REG, RTC_CNTL_ANA_CLK_RTC_SEL_M, RTC_CNTL_ANA_CLK_RTC_SEL_S ) )
{
}

SlowClockRTC::~SlowClockRTC() 
{
    delete src;
}

SlowClockRTC::Source SlowClockRTC::getSource() const 
{ 
    return (Source) src->get(); 
}

void SlowClockRTC::setSource( const SlowClockRTC::Source value ) 
{ 
    src->set( static_cast<uint32_t>( value ) ); 
}
