// Continuous send mode management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlLoop.hpp"

RemoteControlLoop::RemoteControlLoop( const uint32_t channel ) :
    enable( new FlagRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LOOP_CNT_EN_CH0_S ) ),
    repeat( new SubValueRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LOOP_NUM_CH0_M, RMT_TX_LOOP_NUM_CH0_S ) ),
    reset( new FlagRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_LOOP_COUNT_RESET_CH0_S ) )
{
}
    
RemoteControlLoop::~RemoteControlLoop()
{
    delete enable;
    delete repeat;
    delete reset;
}
