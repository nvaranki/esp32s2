// Generator of signal and transmitter of it to the output.
// Includes carrier modulation. Acquires data from the RAM.
//
// © 2021 Nikolai Varankine

#include "soc/gpio_sig_map.h"
#include "soc/rmt_reg.h"
#include "RemoteControlTransmitter.hpp"

RemoteControlTransmitter::RemoteControlTransmitter( const size_t channel ) :
    address( new RemoteControlTransmitterAddress( channel ) ),
    carrier( new RemoteControlTransmitterCarrier( channel ) ),
    loop( new RemoteControlLoop( channel ) ),
    idle( new RemoteControlIdle( channel ) ),
    limit( new RemoteControlLimit( channel ) ),
    send(
        new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_START_CH0_S ),
        new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_STOP_CH0_S ) ),
    repeat( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_CONTI_MODE_CH0_S ) ),
    simultaneously( new FlagRW( RMT_TX_SIM_REG, RMT_TX_SIM_CH0_S + channel ) ),
    interrupt( new InterruptController( // bits 0 3 6 9
        new FlagRO( RMT_INT_RAW_REG, RMT_CH0_TX_END_INT_RAW_S + 0x3 * channel ),
        new FlagRO( RMT_INT_ST_REG,  RMT_CH0_TX_END_INT_ST_S  + 0x3 * channel ),
        new FlagRW( RMT_INT_ENA_REG, RMT_CH0_TX_END_INT_ENA_S + 0x3 * channel ),
        new FlagWO( RMT_INT_CLR_REG, RMT_CH0_TX_END_INT_CLR_S + 0x3 * channel ) ) ),
    index( RMT_SIG_OUT0_IDX + channel )
{
}

RemoteControlTransmitter::~RemoteControlTransmitter()
{
    delete address;
    delete carrier;
    delete loop;
    delete idle;
    delete limit;
    delete send.on;
    delete send.off;
    delete repeat;
    delete simultaneously;
    delete interrupt;
}
