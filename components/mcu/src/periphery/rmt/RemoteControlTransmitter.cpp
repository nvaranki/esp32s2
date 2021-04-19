// Generator of signal and transmitter of it to the output.
// Includes carrier modulation. Acquires data from the RAM.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlTransmitter.hpp"

RemoteControlTransmitter::RemoteControlTransmitter( const size_t channel ) :
    send(
        new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_START_CH0_S ),
        new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_STOP_CH0_S ) ),
    entries( new SubValueRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LIM_CH0_M, RMT_TX_LIM_CH0_S ) ),
    repeat( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_TX_CONTI_MODE_CH0_S ) ),
    simultaneously( new FlagRW( RMT_TX_SIM_REG, RMT_TX_SIM_CH0_S + channel ) ),
    address( channel ),
    carrier( channel ),
    loop( channel ),
    idle( channel )
{
}

RemoteControlTransmitter::~RemoteControlTransmitter()
{
    delete send.on;
    delete send.off;
    delete entries;
    delete repeat;
    delete simultaneously;
}

RemoteControlTransmitter::Address::Address( const size_t channel ) :
    offset( new SubValueRW( RMT_CH0ADDR_REG + 0x4 * channel, RMT_APB_MEM_RADDR_CH0_M, RMT_APB_MEM_RADDR_CH0_S ) ),
    current( new SubValueRW( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_RADDR_EX_CH0_M, RMT_MEM_RADDR_EX_CH0_S ) ),
    reset( new FlagWO( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_RD_RST_CH0_S ) ),
    error( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_APB_MEM_RD_ERR_CH0_S ) ),
    full( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_EMPTY_CH0_S ) )
{
}
    
RemoteControlTransmitter::Address::~Address()
{
    delete offset;
    delete current;
    delete reset;
    delete error;
    delete full;
}

RemoteControlTransmitter::Loop::Loop( const size_t channel ) :
    enable( new FlagRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LOOP_CNT_EN_CH0_S ) ),
    repeat( new SubValueRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_TX_LOOP_NUM_CH0_M, RMT_TX_LOOP_NUM_CH0_S ) ),
    reset( new FlagRW( RMT_CH0_TX_LIM_REG + 0x4 * channel, RMT_LOOP_COUNT_RESET_CH0_S ) )
{
}
    
RemoteControlTransmitter::Loop::~Loop()
{
    delete enable;
    delete repeat;
    delete reset;
}

RemoteControlTransmitter::Idle::Idle( const size_t channel ) :
    output( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_IDLE_OUT_EN_CH0_S ) ),
    level( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_IDLE_OUT_LV_CH0_S ) )
{
}
    
RemoteControlTransmitter::Idle::~Idle()
{
    delete output;
    delete level;
}

RemoteControlTransmitter::Carrier::Carrier( const size_t channel ) :
    modulation( new FlagRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_CARRIER_EN_CH0_S ) ),
    limited( new FlagRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_CARRIER_EFF_EN_CH0_S ) ),
    level( new FlagRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_CARRIER_OUT_LV_CH0_S ) ),
    low ( new SubValueRW( RMT_CH0CARRIER_DUTY_REG + 0x4 * channel, RMT_CARRIER_LOW_CH0_M, RMT_CARRIER_LOW_CH0_S ) ),
    high( new SubValueRW( RMT_CH0CARRIER_DUTY_REG + 0x4 * channel, RMT_CARRIER_HIGH_CH0_M, RMT_CARRIER_HIGH_CH0_S ) )
{
}
    
RemoteControlTransmitter::Carrier::~Carrier()
{
    delete modulation;
    delete limited;
    delete level;
}
