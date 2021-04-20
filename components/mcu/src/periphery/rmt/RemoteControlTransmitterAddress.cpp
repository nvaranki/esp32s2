// Memory management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlTransmitterAddress.hpp"

RemoteControlTransmitterAddress::RemoteControlTransmitterAddress( const uint32_t channel ) :
    offset( new SubValueRW( RMT_CH0ADDR_REG + 0x4 * channel, RMT_APB_MEM_RADDR_CH0_M, RMT_APB_MEM_RADDR_CH0_S ) ),
    current( new SubValueRW( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_RADDR_EX_CH0_M, RMT_MEM_RADDR_EX_CH0_S ) ),
    reset( new FlagWO( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_RD_RST_CH0_S ) ),
    error( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_APB_MEM_RD_ERR_CH0_S ) ),
    full( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_EMPTY_CH0_S ) )
{
}
    
RemoteControlTransmitterAddress::~RemoteControlTransmitterAddress()
{
    delete offset;
    delete current;
    delete reset;
    delete error;
    delete full;
}
