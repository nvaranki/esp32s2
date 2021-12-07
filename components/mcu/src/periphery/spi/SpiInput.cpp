#include "SpiInput.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiInput::SpiInput( const uint32_t rbo ) :
    enable( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_MISO_S ) ),
    length( new SubValueRW( PeriBus1 + rbo + SPI_MISO_DLEN_REG_A, SPI_USR_MISO_DBITLEN_M, SPI_USR_MISO_DBITLEN_S ) ),
    bitOrder( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_RD_BYTE_ORDER_S ) )
{
}

SpiInput::~SpiInput()
{
    delete enable;
    delete length;
    delete bitOrder;
}
