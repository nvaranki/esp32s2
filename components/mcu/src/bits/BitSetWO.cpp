// 32 bit command (write only) register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "BitSetWO.hpp"

void BitSetWO::set( const uint32_t mask, const bool value )
{
    uint32_t m = mask & this->view;
    if( m )
    {
        if( value )
            REG_SET_BIT( this->addr, m );
        else
            REG_CLR_BIT( this->addr, m );
    }
}
