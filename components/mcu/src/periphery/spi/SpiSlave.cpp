#include "SpiSlave.hpp"
#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiSlave::SpiSlave( const uint32_t rbo ) :
    userAddress( new WordRW( PeriBus1 + rbo + SPI_ADDR_REG_A ) ),
    polarityTSCK( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_TSCK_I_EDGE_S ) ),
    polarityRSCK( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_RSCK_I_EDGE_S ) )
{
}

SpiSlave::~SpiSlave()
{
    delete userAddress;
    delete polarityTSCK;
    delete polarityRSCK;
}
