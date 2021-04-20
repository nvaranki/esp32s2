// An I/O channel, part of remote control controller.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "RemoteControlChannel.hpp"

RemoteControlChannel::RemoteControlChannel( const uint32_t channel ) :
    receiver( nullptr ),
    transmitter( nullptr ),
    clock( nullptr ),
    memory( nullptr ),
    carrier( new FlagRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_CARRIER_EN_CH0_S ) ),
    status( new SubValueRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_STATE_CH0_M, RMT_STATE_CH0_S ) ),
    id( channel )
{
}

RemoteControlChannel::~RemoteControlChannel()
{
    if( receiver    != nullptr ) delete receiver;
    if( transmitter != nullptr ) delete transmitter;
    if( clock  != nullptr ) delete clock;
    if( memory != nullptr ) delete memory;
    delete carrier;
    delete status;
}

RemoteControlReceiver* RemoteControlChannel::getReceiver() 
{ 
    if( receiver == nullptr ) 
        receiver = new RemoteControlReceiver( id );
    return receiver; 
}

RemoteControlTransmitter* RemoteControlChannel::getTransmitter() 
{ 
    if( transmitter == nullptr ) 
        transmitter = new RemoteControlTransmitter( id );
    return transmitter; 
}

RemoteControlClock* RemoteControlChannel::getClock() 
{ 
    if( clock == nullptr ) 
        clock = new RemoteControlClock( id );
    return clock; 
}

RemoteControlChannelMemory* RemoteControlChannel::getMemory() 
{ 
    if( memory == nullptr ) 
        memory = new RemoteControlChannelMemory( id );
    return memory; 
}
