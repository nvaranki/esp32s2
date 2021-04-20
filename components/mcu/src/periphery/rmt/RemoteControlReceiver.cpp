// Receiver of signal/modulation and decoder of it to the signal.
// Includes carrier remover. Trasmits data to the RAM.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlReceiver.hpp"

RemoteControlReceiver::RemoteControlReceiver( const uint32_t channel ) :
    address( new RemoteControlReceiverAddress( channel ) ),
    filter( new RemoteControlFilter( channel ) ),
    carrier( new RemoteControlReceiverCarrier( channel ) ),
    enable( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_RX_EN_CH0_S ) ),
    idle( new SubValueRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_IDLE_THRES_CH0_M, RMT_IDLE_THRES_CH0_S ) )
{
}

RemoteControlReceiver::~RemoteControlReceiver()
{
    delete address;
    delete filter;
    delete carrier;
    delete enable;
    delete idle;
}
