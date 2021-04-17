// Pulse counter.
//
// © 2021 Nikolai Varankine

#include "soc/pcnt_reg.h"
#include "PulseCountUnit.hpp"

PulseCountUnit::PulseCountUnit( const size_t id ) :
    value( new SubValueRO(  PCNT_U0_CNT_REG + id * 0x4, PCNT_PULSE_CNT_U0_M, PCNT_PULSE_CNT_U0_S ) ),
    reset( new FlagRW( PCNT_CTRL_REG, PCNT_PULSE_CNT_RST_U0_S + id * 2 ) ),
    pause( new FlagRW( PCNT_CTRL_REG, PCNT_CNT_PAUSE_U0_S + id * 2 ) ),
    status( new SubValueRO( PCNT_U0_STATUS_REG + 0x4 * id, 
        PCNT_CNT_THR_ZERO_MODE_U0_M, PCNT_CNT_THR_ZERO_MODE_U0_S ) ),
    filter( nullptr ),
    comparator( nullptr ),
    id( id )
{
    for( int i = 0; i < MAX_CHANNEL; i++ ) channel[i] = nullptr;
}
    
PulseCountUnit::~PulseCountUnit()
{
    delete value;
    delete reset;
    delete pause;
    delete status;
    for( int i = 0; i < MAX_CHANNEL; i++ ) if( channel[i] != nullptr ) delete channel[i];
    if( comparator != nullptr ) delete comparator;
}

int16_t PulseCountUnit::getValue() const
{
    return (int16_t) value->get(); // +- 16 bit counter
}

PulseCountUnit::Status PulseCountUnit::getStatus() const
{
    return (Status) status->get();
}

PulseCountChannel* PulseCountUnit::getChannel( const size_t i )
{
    if( i >= MAX_CHANNEL ) return nullptr;
    if( channel[i] == nullptr ) channel[i] = new PulseCountChannel( id, i );
    return channel[i];
}

PulseCountFilter* PulseCountUnit::getFilter()
{
    if( filter == nullptr ) filter = new PulseCountFilter( id );
    return filter;
}

PulseCountComparator* PulseCountUnit::getComparator()
{
    if( comparator == nullptr ) comparator = new PulseCountComparator( id );
    return comparator;
}
