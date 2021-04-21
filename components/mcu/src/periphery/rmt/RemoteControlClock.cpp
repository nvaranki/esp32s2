// Clock management, part of remote control channel.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlClock.hpp"

RemoteControlClock::RemoteControlClock( const size_t channel ) :
    source( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_REF_ALWAYS_ON_CH0_S ) ),
    divider( new SubValueRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_DIV_CNT_CH0_M, RMT_DIV_CNT_CH0_S ) ),
    reset( new FlagRW( RMT_REF_CNT_RST_REG, RMT_REF_CNT_RST_CH0_S + channel ) )
{
}

RemoteControlClock::~RemoteControlClock()
{
    delete source;
    delete divider;
    delete reset;
}
