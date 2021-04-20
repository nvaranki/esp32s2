// Carrier demodulator management, part of remote control receiver.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlReceiverCarrier.hpp"

RemoteControlReceiverCarrier::RemoteControlReceiverCarrier( const uint32_t channel ) :
    remove( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_CHK_RX_CARRIER_EN_CH0_S ) ),
    low ( new SubValueRW( RMT_CH0_RX_CARRIER_RM_REG + 0x4 * channel, 
        RMT_CARRIER_LOW_THRES_CH0_M, RMT_CARRIER_LOW_THRES_CH0_S ) ),
    high( new SubValueRW( RMT_CH0_RX_CARRIER_RM_REG + 0x4 * channel, 
        RMT_CARRIER_HIGH_THRES_CH0_M, RMT_CARRIER_HIGH_THRES_CH0_S ) )
{
}
    
RemoteControlReceiverCarrier::~RemoteControlReceiverCarrier()
{
    delete remove;
    delete low;
    delete high;
}
