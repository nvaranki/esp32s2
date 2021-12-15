#include "SpiDMA.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiDMA::SpiDMA( const uint32_t rbo ) :
    receive( new FlagRW( PeriBus1 + rbo + SPI_DMA_IN_LINK_REG_A, SPI_DMA_RX_ENA_S ) ),
    send( new FlagRW( PeriBus1 + rbo + SPI_DMA_OUT_LINK_REG_A, SPI_DMA_TX_ENA_S ) )
{
}

SpiDMA::~SpiDMA()
{
    delete receive;
    delete send;
}
