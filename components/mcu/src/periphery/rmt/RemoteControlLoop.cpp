// Continuous send mode management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlLoop.hpp"

RemoteControlLoop::RemoteControlLoop( const uint32_t channel ) :
    enable( new FlagRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LOOP_CNT_EN_CH0_S ) ),
    repeat( new SubValueRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LOOP_NUM_CH0_M, RMT_TX_LOOP_NUM_CH0_S ) ),
    reset( new FlagRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_LOOP_COUNT_RESET_CH0_S ) ),
    interrupt( new InterruptController( // bits 16 17 18 19
        new FlagRO( RMT_INT_RAW_REG, RMT_CH0_TX_LOOP_INT_RAW_S + 0x1 * channel ),
        new FlagRO( RMT_INT_ST_REG,  RMT_CH0_TX_LOOP_INT_ST_S  + 0x1 * channel ),
        new FlagRW( RMT_INT_ENA_REG, RMT_CH0_TX_LOOP_INT_ENA_S + 0x1 * channel ),
        new FlagWO( RMT_INT_CLR_REG, RMT_CH0_TX_LOOP_INT_CLR_S + 0x1 * channel ) ) )
{
}
    
RemoteControlLoop::~RemoteControlLoop()
{
    delete enable;
    delete repeat;
    delete reset;
    delete interrupt;
}
