#include "SpiDMA.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiDMA::SpiDMA( const uint32_t rbo ) 
{
}

SpiDMA::~SpiDMA()
{
}
