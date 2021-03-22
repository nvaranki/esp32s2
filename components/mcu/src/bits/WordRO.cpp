// 32 bit read only register.
//
// © 2021 Nikolai Varankine

#include "WordRO.hpp"

WordRO::WordRO( const uint32_t a ) : 
    addr( (volatile uint32_t*) a ) 
{
}

uint32_t WordRO::get() const
{
    return *addr;
}
