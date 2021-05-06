// Sequential accessor to pulse data memory; part of remote control channel memory.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlFIFO.hpp"

RemoteControlFIFO::RemoteControlFIFO( const uint32_t channel, SubValueRO* const allocated ) :
    allocated( allocated ),
//  data( new WordRW( RMT_CH0DATA_REG + 0x4 * channel ) ), // PeriBus1
    data( new WordRW( 0x60016000 + 0x4 * channel ) ), // PeriBus2
    addressRead ( new SubValueRO( RMT_CH0ADDR_REG + 0x4 * channel, RMT_APB_MEM_RADDR_CH0_M, RMT_APB_MEM_RADDR_CH0_S ) ),
    addressWrite( new SubValueRO( RMT_CH0ADDR_REG + 0x4 * channel, RMT_APB_MEM_WADDR_CH0_M, RMT_APB_MEM_WADDR_CH0_S ) ),
    reset( new FlagWO( RMT_CH0CONF1_REG + 0x8 * channel, RMT_APB_MEM_RST_CH0_S ) ),
    overflowRead ( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_APB_MEM_RD_ERR_CH0_S ) ),
    overflowWrite( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_APB_MEM_WR_ERR_CH0_S ) )
{
}
    
RemoteControlFIFO::~RemoteControlFIFO()
{
    delete data;
    delete addressRead;
    delete addressWrite;
    delete reset;
    delete overflowRead;
    delete overflowWrite;
}
