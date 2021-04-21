// Transmission marker, part of remote control transmitter.
// Used in continuous mode as indicator of the moment to refresh data.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlLimit.hpp"

RemoteControlLimit::RemoteControlLimit( const uint32_t channel ) :
    entries( new SubValueRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LIM_CH0_M, RMT_TX_LIM_CH0_S ) ),
    interrupt( new InterruptController( // bits 12 13 14 15
        new FlagRO( RMT_INT_RAW_REG, RMT_CH0_TX_THR_EVENT_INT_RAW_S + 0x1 * channel ),
        new FlagRO( RMT_INT_ST_REG,  RMT_CH0_TX_THR_EVENT_INT_ST_S  + 0x1 * channel ),
        new FlagRW( RMT_INT_ENA_REG, RMT_CH0_TX_THR_EVENT_INT_ENA_S + 0x1 * channel ),
        new FlagWO( RMT_INT_CLR_REG, RMT_CH0_TX_THR_EVENT_INT_CLR_S + 0x1 * channel ) ) )
{
}
    
RemoteControlLimit::~RemoteControlLimit()
{
    delete entries;
    delete interrupt;
}
