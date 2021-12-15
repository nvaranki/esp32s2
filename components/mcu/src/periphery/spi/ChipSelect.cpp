#include "MicroControllerUnit.hpp"
#include "ChipSelect.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"
    
#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

ChipSelect::ChipSelect( const uint32_t rbo ) :
    setupMode( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CS_SETUP_S ) ),
    setupTime( new SubValueRW( PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_SETUP_TIME_M, SPI_CS_SETUP_TIME_S ) ),
    holdMode( new FlagRW(  PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CS_HOLD_S  ) ),
    holdTime( new SubValueRW(  PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_HOLD_TIME_M,  SPI_CS_HOLD_TIME_S  ) ),
    holdDelay( new SubValueRW(  PeriBus1 + rbo + SPI_CTRL1_REG_A, SPI_CS_HOLD_DELAY_M, SPI_CS_HOLD_DELAY_S  ) ),
    delayMode( new SubValueRW( PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_DELAY_MODE_M, SPI_CS_DELAY_MODE_S ) ),
    delayTime( new SubValueRW( PeriBus1 + rbo + SPI_CTRL2_REG_A, SPI_CS_DELAY_NUM_M,  SPI_CS_DELAY_NUM_S  ) ),
    keepActive( new FlagRW(  PeriBus1 + rbo + SPI_MISC_REG_A, SPI_CS_KEEP_ACTIVE_S ) ),
    inputPolarity( new FlagRW(  PeriBus1 + rbo + SPI_MISC_REG_A, SPI_SLAVE_CS_POL_S ) ),
    outputPolarity( new SubValueRW( PeriBus1 + rbo + SPI_MISC_REG_A, SPI_MASTER_CS_POL_M,  SPI_MASTER_CS_POL_S  ) ),
{
}
    
ChipSelect::~ChipSelect()
{
    delete setupMode;
    delete setupTime;
    delete holdMode;
    delete holdTime;
    delete holdDelay;
    delete delayMode;
    delete delayTime;
    delete keepActive;
    delete inputPolarity;
    delete outputPolarity;
}
