// An I/O channel, part of remote control controller.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "RemoteControlChannel.hpp"

RemoteControlChannel::RemoteControlChannel( const uint32_t channel ) :
    receiver( nullptr ),
    transmitter( nullptr ),
    clock( nullptr ),
    memory( nullptr ),
    fifo( new WordRW( RMT_CH0DATA_REG + 0x4 * channel ) ),
    carrier( new FlagRW( RMT_CH0CONF0_REG + 0x8 * channel, RMT_CARRIER_EN_CH0_S ) ),
    status( new SubValueRO( RMT_CH0STATUS_REG + 0x4 * channel, RMT_STATE_CH0_M, RMT_STATE_CH0_S ) ),
    interrupt( new InterruptController( // bits 2 5 8 11
        new FlagRO( RMT_INT_RAW_REG, RMT_CH0_ERR_INT_RAW_S + 0x3 * channel ),
        new FlagRO( RMT_INT_ST_REG,  RMT_CH0_ERR_INT_ST_S  + 0x3 * channel ),
        new FlagRW( RMT_INT_ENA_REG, RMT_CH0_ERR_INT_ENA_S + 0x3 * channel ),
        new FlagWO( RMT_INT_CLR_REG, RMT_CH0_ERR_INT_CLR_S + 0x3 * channel ) ) ),
    id( channel )
{
}

RemoteControlChannel::~RemoteControlChannel()
{
    if( receiver    != nullptr ) delete receiver;
    if( transmitter != nullptr ) delete transmitter;
    if( clock  != nullptr ) delete clock;
    if( memory != nullptr ) delete memory;
    delete fifo;
    delete carrier;
    delete status;
    delete interrupt;
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
