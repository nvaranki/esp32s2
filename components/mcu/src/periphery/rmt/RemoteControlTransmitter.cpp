// Generator of signal and transmitter of it to the output.
// Includes carrier modulation. Acquires data from the RAM.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlTransmitter.hpp"

RemoteControlTransmitter::RemoteControlTransmitter( const size_t channel ) :
    address( new RemoteControlTransmitterAddress( channel ) ),
    carrier( new RemoteControlTransmitterCarrier( channel ) ),
    loop( new RemoteControlLoop( channel ) ),
    idle( new RemoteControlIdle( channel ) ),
    send(
        new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_START_CH0_S ),
        new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_STOP_CH0_S ) ),
    entries( new SubValueRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LIM_CH0_M, RMT_TX_LIM_CH0_S ) ),
    repeat( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_CONTI_MODE_CH0_S ) ),
    simultaneously( new FlagRW( RMT_TX_SIM_REG, RMT_TX_SIM_CH0_S + channel ) )
{
}

RemoteControlTransmitter::~RemoteControlTransmitter()
{
    delete address;
    delete carrier;
    delete loop;
    delete idle;
    delete send.on;
    delete send.off;
    delete entries;
    delete repeat;
    delete simultaneously;
}
