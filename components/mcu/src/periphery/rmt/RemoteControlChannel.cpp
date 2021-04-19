// An I/O channel of remote control controller.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlChannel.hpp"
//TODO #include "RemoteControlController.hpp"

RemoteControlChannel::RemoteControlChannel( const size_t channel ) :
    receiver( channel ),
    transmitter( channel ),
    carrier( new FlagRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_CARRIER_EN_CH0_S ) ),
    clock( channel ),
    memory( channel ),
    status( new SubValueRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_STATE_CH0_M, RMT_STATE_CH0_S ) ),
    id( channel )
{
}

RemoteControlChannel::~RemoteControlChannel()
{
    delete carrier;
    delete status;
}

RemoteControlChannel::Memory::Memory( const size_t channel ) :
    //TODO allocatedMax( RemoteControlController::MAX_CHANNEL - channel ),
    allocatedMax( 4 - channel ),
    allocated( new SubValueRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_MEM_SIZE_CH0_M, RMT_MEM_SIZE_CH0_S ) ),
    owner( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_MEM_OWNER_CH0_S ) ),
    error( new FlagRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_MEM_OWNER_CH0_S ) )
{
}
    
RemoteControlChannel::Memory::~Memory()
{
    delete allocated;
    delete owner;
    delete error;
}

uint32_t RemoteControlChannel::Memory::getAllocated() const
{
    return allocated->get();
}

void RemoteControlChannel::Memory::setAllocated( const uint32_t n )
{
    if( n > 0 && n <= allocatedMax ) allocated->set( n );
}

RemoteControlChannel::Clock::Clock( const size_t channel ) :
    source( new FlagRW( RMT_CH0CONF1_REG + 0x8 * channel, RMT_REF_ALWAYS_ON_CH0_S ) ),
    divider( new SubValueRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_DIV_CNT_CH0_M, RMT_DIV_CNT_CH0_S ) )
{
}

RemoteControlChannel::Clock::~Clock()
{
    delete source;
    delete divider;
}
