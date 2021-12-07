#include "SpiAddress.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiAddress::SpiAddress( const uint32_t rbo ) :
    enable( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_ADDR_S ) ),
    length( new SubValueRW( PeriBus1 + rbo + SPI_USER1_REG_A, SPI_USR_ADDR_BITLEN_M, SPI_USR_ADDR_BITLEN_S ) ),
    bitOrder( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_WR_BYTE_ORDER_S ) ),
    value( new WordRW( PeriBus1 + rbo + SPI_ADDR_REG_A ) ) //TODO [31:8]:address to slave  [7:0]:Reserved
{
}

SpiAddress::~SpiAddress()
{
    delete enable;
    delete length;
    delete bitOrder;
    delete value;
}
