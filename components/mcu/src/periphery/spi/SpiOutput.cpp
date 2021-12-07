#include "SpiOutput.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiOutput::SpiOutput( const uint32_t rbo ) :
    enable(  new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_MOSI_S ) ),
    length( new SubValueRW( PeriBus1 + rbo + SPI_MOSI_DLEN_REG_A, SPI_USR_MOSI_DBITLEN_M, SPI_USR_MOSI_DBITLEN_S ) ),
    bitOrder( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_WR_BYTE_ORDER_S ) )
{
}

SpiOutput::~SpiOutput()
{
    delete enable;
    delete length;
    delete bitOrder;
}
