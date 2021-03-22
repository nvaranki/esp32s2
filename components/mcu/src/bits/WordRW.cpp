// 32 bit read and write register.
//
// © 2021 Nikolai Varankine

#include "WordRW.hpp"

void WordRW::set( const uint32_t value )
{
    *( this->addr ) = value;
}
