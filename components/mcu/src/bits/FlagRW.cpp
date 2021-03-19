// One bit read and write value within 32 bit register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "FlagRW.hpp"

void FlagRW::set( const bool value )
{
    if( value )
        REG_SET_BIT( this->addr, this->view );
    else
        REG_CLR_BIT( this->addr, this->view );
}
