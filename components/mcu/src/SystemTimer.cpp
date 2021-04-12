// Main 64 bit system timer.
//
// © 2021 Nikolai Varankine

#include "soc/systimer_reg.h"
#include "SystemTimer.hpp"

SystemTimer::SystemTimer() :
    valueLO( new WordRO( SYSTIMER_VALUE_LO_REG ) ),
    valueHI( new WordRO( SYSTIMER_VALUE_HI_REG ) ),
    update( new FlagWO( SYSTIMER_UPDATE_REG, SYSTIMER_TIMER_UPDATE_S ) ),
    valid ( new FlagRO( SYSTIMER_UPDATE_REG, SYSTIMER_TIMER_VALUE_VALID_S ) )
{
}
    
SystemTimer::~SystemTimer()
{
    delete valueLO;
    delete valueHI;
    delete update;
    delete valid;
}

uint64_t SystemTimer::getValue() const
{
    return ( (uint64_t) valueHI->get() << 32 ) | valueLO->get();
}

uint64_t SystemTimer::getValidValue( uint32_t attempts )
{
    update->on();
    while( !valid->get() ) 
        if( attempts-- <= 0 ) return INVALID; 
    return ( (uint64_t) valueHI->get() << 32 ) | valueLO->get();
}
