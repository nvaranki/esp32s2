// Read and write value within 32 bit register.
//
// © 2021 Nikolai Varankine

#include "soc/soc.h"
#include "SubValueRW.hpp"

void SubValueRW::set( const uint32_t value )
{
    REG_CLR_BIT( this->addr, this->view );
    REG_SET_BIT( this->addr, ( value << this->shift ) & this->view );
}
