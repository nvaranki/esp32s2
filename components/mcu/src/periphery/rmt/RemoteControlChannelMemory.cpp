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
    fifo( new WordRW( RMT_CH0DATA_REG + 0x4 * channel ) ),
    reset( new FlagWO( RMT_CH0CONF1_REG + 0x8 * channel, RMT_APB_MEM_RST_CH0_S ) ),
    ram( (volatile uint32_t *) &RMTMEM.chan[ channel ] ) // 0x***16400 + 0x100 * channel
{
}
    
RemoteControlChannelMemory::~RemoteControlChannelMemory()
{
    delete allocated;
    delete owner;
    delete error;
    delete fifo;
    delete reset;
}

void RemoteControlChannelMemory::setAllocated( const uint32_t n )
{
    if( n > 0 && n <= allocatedMax ) ( (SubValueRW*) allocated )->set( n );
}

typedef
    union {
        rmt_item16_t entries[2];
        uint32_t raw;
    }
    rmt_item16x2_t;

rmt_item16_t RemoteControlChannelMemory::getEntry( const uint32_t i )
{
    if( i >= 128 * allocated->get() ) return ENTRY_END; // check max i
    rmt_item16x2_t v2;
    v2.raw = ram[ i >> 1 ];
    return v2.entries[ i & 0x1 ];
}
    
void RemoteControlChannelMemory::setEntry( const uint32_t i, const rmt_item16_t& e )
{
    if( i >= 128 * allocated->get() ) return; // check max i
    rmt_item16x2_t v2;
    const uint32_t i2( i >> 1 );
    v2.raw = ram[ i2 ];
    v2.entries[ i & 0x1 ].val = e.val;
    ram[ i2 ] = v2.raw;
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
