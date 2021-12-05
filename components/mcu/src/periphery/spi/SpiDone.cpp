#include "SpiDone.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiDone::SpiDone( const uint32_t rbo ) :
    holdCS( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CS_HOLD_S ) ),
    length( new SubValueRW( PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_HOLD_TIME_M, SPI_CS_HOLD_TIME_S ) )
{
}

SpiDone::~SpiDone()
{
    delete holdCS;
    delete length;
}
