#include "MicroControllerUnit.hpp"
#include "ChipSelect.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"
    
#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

ChipSelect::ChipSelect( const uint32_t rbo ) :
    setupMode( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CS_SETUP_S ) ),
    holdMode( new FlagRW(  PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CS_HOLD_S  ) ),
    setupTime( new SubValueRW( PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_SETUP_TIME_M, SPI_CS_SETUP_TIME_S ) ),
    holdTime( new SubValueRW(  PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_HOLD_TIME_M,  SPI_CS_HOLD_TIME_S  ) ),
    delayMode( new SubValueRW( PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_DELAY_MODE_M, SPI_CS_DELAY_MODE_S ) ),
    delayTime( new SubValueRW( PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_DELAY_NUM_M,  SPI_CS_DELAY_NUM_S  ) )
{
}
    
ChipSelect::~ChipSelect()
{
    delete setupMode;
    delete holdMode;
    delete setupTime;
    delete holdTime;
    delete delayMode;
    delete delayTime;
}
