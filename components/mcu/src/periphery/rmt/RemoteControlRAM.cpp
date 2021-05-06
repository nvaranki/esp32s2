// Indexed accessor to pulse data memory; part of remote control channel memory.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlRAM.hpp"
#include "RemoteControlChannelMemory.hpp"

RemoteControlRAM::RemoteControlRAM( const uint32_t channel, SubValueRO* const allocated ) :
    allocated( allocated ),
    // ram( (volatile uint32_t *) &RMTMEM.chan[ channel ] ), // PeriBus1 0x***16400 + 0x100 * channel
    ram( (volatile uint32_t *) (0x60016400 + 0x100 * channel) ), // PeriBus2 0x***16400 + 0x100 * channel
    addressRead ( new SubValueRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_RADDR_EX_CH0_M, RMT_MEM_RADDR_EX_CH0_S ) ),
    addressWrite( new SubValueRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_WADDR_EX_CH0_M, RMT_MEM_WADDR_EX_CH0_S ) ),
    resetRead ( new FlagWO( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_RD_RST_CH0_S ) ),
    resetWrite( new FlagWO( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_WR_RST_CH0_S ) ),
    overflowRead ( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_EMPTY_CH0_S ) ),
    overflowWrite( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_FULL_CH0_S ) )
{
}
    
RemoteControlRAM::~RemoteControlRAM()
{
    delete addressRead;
    delete addressWrite;
    delete resetRead;
    delete resetWrite;
    delete overflowRead;
    delete overflowWrite;
}
    
typedef
    union {
        rmt_item16_t entries[2];
        uint32_t raw;
    }
    rmt_item16x2_t;

rmt_item16_t RemoteControlRAM::getEntry( const uint32_t i )
{
    if( i >= 128 * allocated->get() ) return RemoteControlChannelMemory::ENTRY_END; // check max i
    rmt_item16x2_t v2;
    v2.raw = ram[ i >> 1 ];
    return v2.entries[ i & 0x1 ];
}
    
void RemoteControlRAM::setEntry( const uint32_t i, const rmt_item16_t& e )
{
    if( i >= 128 * allocated->get() ) return; // check max i
    rmt_item16x2_t v2;
    const uint32_t i2( i >> 1 );
    v2.raw = ram[ i2 ];
    v2.entries[ i & 0x1 ].val = e.val;
    ram[ i2 ] = v2.raw;
}

rmt_item32_t RemoteControlRAM::getEntry2( const uint32_t i )
{
    if( i >= 64 * allocated->get() ) // check max i
        return RemoteControlChannelMemory::entry2(
            RemoteControlChannelMemory::ENTRY_END,
            RemoteControlChannelMemory::ENTRY_END );
    rmt_item32_t v2;
    v2.val = ram[ i ];
    return v2;
}
    
void RemoteControlRAM::setEntry2( const uint32_t i, const rmt_item32_t& e )
{
    if( i >= 64 * allocated->get() ) return; // check max i
    ram[ i ] = e.val;
}
