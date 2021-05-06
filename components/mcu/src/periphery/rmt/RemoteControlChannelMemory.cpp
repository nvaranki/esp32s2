// Pulse data memory access management; part of remote control channel.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlChannelMemory.hpp"
#include "RemoteControlController.hpp"

const rmt_item16_t RemoteControlChannelMemory::ENTRY_END = RemoteControlChannelMemory::entry( false, 0 );

RemoteControlChannelMemory::RemoteControlChannelMemory( const uint32_t channel ) :
    allocatedMax( RemoteControlController::MAX_CHANNEL - channel ),
    allocated( new SubValueRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_MEM_SIZE_CH0_M, RMT_MEM_SIZE_CH0_S ) ),
    owner( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_OWNER_CH0_S ) ),
    error( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_OWNER_CH0_S ) ),
    ram( new RemoteControlRAM( channel, allocated ) ),
    fifo( new RemoteControlFIFO( channel, allocated ) )
{
}
    
RemoteControlChannelMemory::~RemoteControlChannelMemory()
{
    delete allocated;
    delete owner;
    delete error;
    delete ram;
    delete fifo;
}

void RemoteControlChannelMemory::setAllocated( const uint32_t n )
{
    if( n > 0 && n <= allocatedMax ) ( (SubValueRW*) allocated )->set( n );
}

rmt_item16_t RemoteControlChannelMemory::entry( const bool level, const uint16_t duration ) 
{ 
    rmt_item16_t v;
    v.level    = level;
    v.duration = duration > 0x7FFF ? 0 /*TODO error */ : duration;
    return v;
}

rmt_item32_t RemoteControlChannelMemory::entry2( const rmt_item16_t& e0, const rmt_item16_t& e1 )
{ 
    rmt_item32_t v;
    v.level0    = e0.level;
    v.duration0 = e0.duration > 0x7FFF ? 0 /*TODO error */ : e0.duration;
    v.level1    = e1.level;
    v.duration1 = e1.duration > 0x7FFF ? 0 /*TODO error */ : e1.duration;
    return v;
}
