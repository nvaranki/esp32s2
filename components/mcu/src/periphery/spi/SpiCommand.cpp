#include "SpiCommand.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiCommand::SpiCommand( const uint32_t rbo ) :
    enable( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_COMMAND_S ) ),
    length( new SubValueRW( PeriBus1 + rbo + SPI_USER2_REG_A, SPI_USR_COMMAND_BITLEN_M, SPI_USR_COMMAND_BITLEN_S ) ),
    bitOrder( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_WR_BYTE_ORDER_S ) ),
    value(  new SubValueRW( PeriBus1 + rbo + SPI_USER2_REG_A, SPI_USR_COMMAND_VALUE_M,  SPI_USR_COMMAND_VALUE_S  ) )
{
}

SpiCommand::~SpiCommand()
{
    delete enable;
    delete length;
    delete bitOrder;
    delete value;
}
