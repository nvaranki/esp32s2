// Memory management, part of remote control receiver.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlReceiverAddress.hpp"

RemoteControlReceiverAddress::RemoteControlReceiverAddress( const uint32_t channel ) :
    fifo( new SubValueRO( RMT_CH0ADDR_REG + 0x4 * channel, RMT_APB_MEM_WADDR_CH0_M, RMT_APB_MEM_WADDR_CH0_S ) ),
    ram( new SubValueRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_WADDR_EX_CH0_M, RMT_MEM_WADDR_EX_CH0_S ) ),
    reset( new FlagWO( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_WR_RST_CH0_S ) ),
    error( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_APB_MEM_WR_ERR_CH0_S ) ),
    full( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_FULL_CH0_S ) )
{
}
    
RemoteControlReceiverAddress::~RemoteControlReceiverAddress()
{
    delete fifo;
    delete ram;
    delete reset;
    delete error;
    delete full;
}
