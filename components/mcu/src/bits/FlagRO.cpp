// One bit read only value within 32 bit register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "FlagRO.hpp"

FlagRO::FlagRO( const uint32_t a, const uint8_t s ) : 
    addr( (volatile uint32_t*) a ), view( BIT(s) ) 
{
}

bool FlagRO::get() const
{
    return REG_GET_BIT( addr, view );
}
