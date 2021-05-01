// Carrier modulator management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlTransmitterCarrier.hpp"

RemoteControlTransmitterCarrier::RemoteControlTransmitterCarrier( const uint32_t channel ) :
    enable( new FlagRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_CARRIER_EN_CH0_S ) ),
    limited(  new FlagRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_CARRIER_EFF_EN_CH0_S ) ),
    low ( new SubValueRW( RMT_CH0CARRIER_DUTY_REG + 0x4 * channel, RMT_CARRIER_LOW_CH0_M, RMT_CARRIER_LOW_CH0_S ) ),
    high( new SubValueRW( RMT_CH0CARRIER_DUTY_REG + 0x4 * channel, RMT_CARRIER_HIGH_CH0_M, RMT_CARRIER_HIGH_CH0_S ) )
{
}
    
RemoteControlTransmitterCarrier::~RemoteControlTransmitterCarrier()
{
    delete enable;
    delete limited;
    delete low;
    delete high;
}
