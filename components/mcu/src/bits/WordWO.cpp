// 32 bit command (write only) register.
//
// © 2021 Nikolai Varankine

#include "WordWO.hpp"

WordWO::WordWO( const uint32_t a ) : 
    addr( (volatile uint32_t*) a )
{
}

void WordWO::set( const uint32_t value )
{
    *( this->addr ) = value;
}
