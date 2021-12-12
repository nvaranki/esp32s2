#include "SpiDone.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiDone::SpiDone( const uint32_t rbo, const ChipSelect* cs ) :
    holdCS( cs->holdMode ),
    length( cs->holdTime )
{
}

SpiDone::~SpiDone()
{
}
