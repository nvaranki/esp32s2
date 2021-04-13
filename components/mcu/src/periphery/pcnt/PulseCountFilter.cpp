// Input pulse filter.
// Applies to both channels.
//
// © 2021 Nikolai Varankine

#include "soc/pcnt_reg.h"
#include "PulseCountFilter.hpp"

PulseCountFilter::PulseCountFilter( const size_t unit ) :
    enable( new FlagRW( PCNT_U0_CONF0_REG + 0xC * unit, PCNT_FILTER_EN_U0_S ) ),
    threshold( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, PCNT_FILTER_THRES_U0_M, PCNT_FILTER_THRES_U0_S ) )
{
}
    
PulseCountFilter::~PulseCountFilter()
{
    delete enable;
    delete threshold;
}
