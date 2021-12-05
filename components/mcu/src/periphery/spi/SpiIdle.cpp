#include "SpiIdle.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiIdle::SpiIdle( const uint32_t rbo ) :
    enable( new FlagRW( PeriBus1 + rbo + SPI_CMD_REG_A, SPI_USR_S ) )
{
}

SpiIdle::~SpiIdle()
{
    delete enable;
}
