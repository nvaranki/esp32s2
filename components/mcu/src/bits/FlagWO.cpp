// One bit command (write only) value within 32 bit register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "FlagWO.hpp"

FlagWO::FlagWO( const uint32_t a, const uint8_t s ) : 
    addr( (volatile uint32_t*) a ), view( BIT(s) ) 
{
}

void FlagWO::set( const bool value )
{
    if( value )
        REG_SET_BIT( this->addr, this->view );
    else
        REG_CLR_BIT( this->addr, this->view );
}
