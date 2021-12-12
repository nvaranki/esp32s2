#include "SpiPrepare.hpp"
#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiPrepare::SpiPrepare( const uint32_t rbo, const ChipSelect* cs ) :
    enable( cs->setupMode ),
    duration( cs->setupTime )
{
}

SpiPrepare::~SpiPrepare()
{
}
