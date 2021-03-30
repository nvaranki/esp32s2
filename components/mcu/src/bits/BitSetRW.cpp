// 32 bit read and write register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "BitSetRW.hpp"

void BitSetRW::set( const uint32_t mask, const bool value )
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

void BitSetRW::setAll( const bool value )
{
    if( value )
        *this->addr |= this->view;
    else
        *this->addr &= ~this->view;
}
