// General manager of entire pulse data memory, part of remote control controller.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlMemory.hpp"
    
RemoteControlMemory::RemoteControlMemory() :
    clock ( new FlagRW( RMT_APB_CONF_REG, RMT_MEM_CLK_FORCE_ON_S ) ),
    wrap  ( new FlagRW( RMT_APB_CONF_REG, RMT_MEM_TX_WRAP_EN_S ) ),
    direct( new FlagRW( RMT_APB_CONF_REG, RMT_APB_FIFO_MASK_S ) ),
    power( 
        new FlagRW( RMT_APB_CONF_REG, RMT_MEM_FORCE_PU_S ),
        new FlagRW( RMT_APB_CONF_REG, RMT_MEM_FORCE_PD_S ) )
{
}

RemoteControlMemory::~RemoteControlMemory()
{
    delete clock;
    delete wrap;
    delete direct;
    delete power.on;
    delete power.off;
}
