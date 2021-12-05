#include "SpiDummy.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiDummy::SpiDummy( const uint32_t rbo ) :
    enable( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_DUMMY_S ) ),
    length( new SubValueRW( PeriBus1 + rbo + SPI_USER1_REG_A, SPI_USR_DUMMY_CYCLELEN_M, SPI_USR_DUMMY_CYCLELEN_S ) ),
    level( new FlagRW( PeriBus1 + rbo + SPI_CTRL0_REG_A, SPI_DUMMY_OUT_S ) ),

    disableDummyClock( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_DUMMY_IDLE_S ) ),
    hold( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_DUMMY_HOLD_S ) ),
    holdCS( new FlagRO( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CS_HOLD_S ) )
{
}

SpiDummy::~SpiDummy()
{
    delete enable;
    delete length;
    delete level;

    delete disableDummyClock;
    delete hold;
    delete holdCS;
}
