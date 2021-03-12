// Read only value within 32 bit register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "ValueRO.hpp"

ValueRO::ValueRO( const uint32_t a, const uint32_t v, const uint8_t s ) : 
    addr( a ), view( v ), shift( s ) 
{
    //TODO check shift vs view
}

uint32_t ValueRO::get() const
{
    return REG_GET_BIT( addr, view ) >> shift;
}
