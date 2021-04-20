// Idle mode management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlIdle.hpp"

RemoteControlIdle::RemoteControlIdle( const uint32_t channel ) :
    output( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_IDLE_OUT_EN_CH0_S ) ),
    level( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_IDLE_OUT_LV_CH0_S ) )
{
}
    
RemoteControlIdle::~RemoteControlIdle()
{
    delete output;
    delete level;
}
