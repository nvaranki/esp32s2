// Receiver of signal/modulation and decoder of it to the signal.
// Includes carrier remover. Trasmits data to the RAM.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlReceiver.hpp"

RemoteControlReceiver::RemoteControlReceiver( const size_t channel ) :
    enable( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_RX_EN_CH0_S ) ),
    read( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_CHK_RX_CARRIER_EN_CH0_S ) ),
    idle( new SubValueRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_IDLE_THRES_CH0_M, RMT_IDLE_THRES_CH0_S ) ),
    address( channel ),
    filter( channel ),
    carrier( channel )
{
}

RemoteControlReceiver::~RemoteControlReceiver()
{
    delete enable;
    delete read;
    delete idle;
}

RemoteControlReceiver::Address::Address( const size_t channel ) :
    offset( new SubValueRW( RMT_CH0ADDR_REG + 0x4 * channel, RMT_APB_MEM_WADDR_CH0_M, RMT_APB_MEM_WADDR_CH0_S ) ),
    current( new SubValueRW( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_WADDR_EX_CH0_M, RMT_MEM_WADDR_EX_CH0_S ) ),
    reset( new FlagWO( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_WR_RST_CH0_S ) ),
    error( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_APB_MEM_WR_ERR_CH0_S ) ),
    full( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_FULL_CH0_S ) )
{
}
    
RemoteControlReceiver::Address::~Address()
{
    delete offset;
    delete current;
    delete reset;
    delete error;
    delete full;
}

RemoteControlReceiver::Filter::Filter( const size_t channel ) :
    enable( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_RX_FILTER_EN_CH0_S ) ),
    threshold( new SubValueRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_RX_FILTER_THRES_CH0_M, RMT_RX_FILTER_THRES_CH0_S ) )
{
}
    
RemoteControlReceiver::Filter::~Filter()
{
    delete enable;
    delete threshold;
}

RemoteControlReceiver::Carrier::Carrier( const size_t channel ) :
    low ( new SubValueRW( RMT_CH0_RX_CARRIER_RM_REG + 0x4 * channel, RMT_CARRIER_LOW_THRES_CH0_M, RMT_CARRIER_LOW_THRES_CH0_S ) ),
    high( new SubValueRW( RMT_CH0_RX_CARRIER_RM_REG + 0x4 * channel, RMT_CARRIER_HIGH_THRES_CH0_M, RMT_CARRIER_HIGH_THRES_CH0_S ) )
{
}
    
RemoteControlReceiver::Carrier::~Carrier()
{
    delete low;
    delete high;
}
