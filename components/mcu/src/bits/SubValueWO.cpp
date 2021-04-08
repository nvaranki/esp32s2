// Command (write only) value within 32 bit register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "SubValueWO.hpp"

SubValueWO::SubValueWO( const uint32_t a, const uint32_t v, const uint8_t s ) : 
    addr( (volatile uint32_t*) a ), view( v ), shift( s ) 
{
    //TODO check shift vs view
}

void SubValueWO::set( const uint32_t value )
{
    REG_CLR_BIT( this->addr, this->view );
    REG_SET_BIT( this->addr, ( value << this->shift ) & this->view );
}
