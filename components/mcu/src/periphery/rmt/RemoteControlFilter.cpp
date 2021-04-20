// Input sinal filter management, part of remote control receiver.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlFilter.hpp"

RemoteControlFilter::RemoteControlFilter( const uint32_t channel ) :
    enable( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_RX_FILTER_EN_CH0_S ) ),
    threshold( new SubValueRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_RX_FILTER_THRES_CH0_M, RMT_RX_FILTER_THRES_CH0_S ) )
{
}
    
RemoteControlFilter::~RemoteControlFilter()
{
    delete enable;
    delete threshold;
}
