// Read only value within 32 bit register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "SubValueRO.hpp"

SubValueRO::SubValueRO( const uint32_t a, const uint32_t v, const uint8_t s ) : 
    addr( (volatile uint32_t*) a ), view( v ), shift( s ) 
{
    //TODO check shift vs view
}

uint32_t SubValueRO::get() const
{
    return REG_GET_BIT( addr, view ) >> shift;
}
