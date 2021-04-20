// Pulse data memory access management; part of remote control channel.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlChannelMemory.hpp"
#include "RemoteControlController.hpp"

RemoteControlChannelMemory::RemoteControlChannelMemory( const uint32_t channel ) :
    allocatedMax( RemoteControlController::MAX_CHANNEL - channel ),
    allocated( new SubValueRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_MEM_SIZE_CH0_M, RMT_MEM_SIZE_CH0_S ) ),
    owner( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_OWNER_CH0_S ) ),
    error( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_OWNER_CH0_S ) )
{
}
    
RemoteControlChannelMemory::~RemoteControlChannelMemory()
{
    delete allocated;
    delete owner;
    delete error;
}

uint32_t RemoteControlChannelMemory::getAllocated() const
{
    return allocated->get();
}

void RemoteControlChannelMemory::setAllocated( const uint32_t n )
{
    if( n > 0 && n <= allocatedMax ) allocated->set( n );
}
