// 32 bit read only register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "BitSetRO.hpp"

bool BitSetRO::get( const uint32_t test ) const
{
    uint32_t t = test & view;
    return t && !( t ^ REG_GET_BIT( addr, t ) );
}

uint32_t BitSetRO::getAll() const
{
    return REG_GET_BIT( addr, view );
}
