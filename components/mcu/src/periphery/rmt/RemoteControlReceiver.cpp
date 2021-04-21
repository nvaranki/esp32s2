// Receiver of signal/modulation and decoder of it to the signal.
// Includes carrier remover. Trasmits data to the RAM.
//
// © 2021 Nikolai Varankine

#include "soc/gpio_sig_map.h"
#include "soc/rmt_reg.h"
#include "RemoteControlReceiver.hpp"

RemoteControlReceiver::RemoteControlReceiver( const uint32_t channel ) :
    address( new RemoteControlReceiverAddress( channel ) ),
    filter( new RemoteControlFilter( channel ) ),
    carrier( new RemoteControlReceiverCarrier( channel ) ),
    enable( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_RX_EN_CH0_S ) ),
    idle( new SubValueRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_IDLE_THRES_CH0_M, RMT_IDLE_THRES_CH0_S ) ),
    interrupt( new InterruptController( // bits 1 4 7 10
        new FlagRO( RMT_INT_RAW_REG, RMT_CH0_RX_END_INT_RAW_S + 0x3 * channel ),
        new FlagRO( RMT_INT_ST_REG,  RMT_CH0_RX_END_INT_ST_S  + 0x3 * channel ),
        new FlagRW( RMT_INT_ENA_REG, RMT_CH0_RX_END_INT_ENA_S + 0x3 * channel ),
        new FlagWO( RMT_INT_CLR_REG, RMT_CH0_RX_END_INT_CLR_S + 0x3 * channel ) ) ),
    index( RMT_SIG_IN0_IDX + channel )
{
}

RemoteControlReceiver::~RemoteControlReceiver()
{
    delete address;
    delete filter;
    delete carrier;
    delete enable;
    delete idle;
    delete interrupt;
}
