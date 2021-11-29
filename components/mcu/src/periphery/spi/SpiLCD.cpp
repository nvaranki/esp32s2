#include "SpiLCD.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiLCD::SpiLCD( const uint32_t rbo ) :
    enable( new FlagRW( PeriBus1 + rbo + SPI_LCD_CTRL0_REG_A, SPI_LCD_MODE_EN_S ) ),
    blankFrontPorchH( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL0_REG_A, SPI_LCD_HB_FRONT_M, SPI_LCD_HB_FRONT_S ) ),
    blankFrontPorchV( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL1_REG_A, SPI_LCD_VB_FRONT_M, SPI_LCD_VB_FRONT_S ) ),
    totalH( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL1_REG_A, SPI_LCD_HT_WIDTH_M, SPI_LCD_HT_WIDTH_S ) ),
    totalV( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL0_REG_A, SPI_LCD_VT_HEIGHT_M, SPI_LCD_VT_HEIGHT_S ) ),
    activeH( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL1_REG_A, SPI_LCD_HA_WIDTH_M, SPI_LCD_HA_WIDTH_S ) ),
    activeV( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL0_REG_A, SPI_LCD_VA_HEIGHT_M, SPI_LCD_VA_HEIGHT_S ) ),
    idlePolarityH( new FlagRW( PeriBus1 + rbo + SPI_LCD_CTRL2_REG_A, SPI_HSYNC_IDLE_POL_S ) ),
    idlePolarityV( new FlagRW( PeriBus1 + rbo + SPI_LCD_CTRL2_REG_A, SPI_VSYNC_IDLE_POL_S ) ),
    widthSyncH( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL2_REG_A, SPI_LCD_HSYNC_WIDTH_M, SPI_LCD_HSYNC_WIDTH_S ) ),
    widthSyncV( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL2_REG_A, SPI_LCD_VSYNC_WIDTH_M, SPI_LCD_VSYNC_WIDTH_S ) ),
    positionSyncH( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL2_REG_A, SPI_LCD_HSYNC_POSITION_M, SPI_LCD_HSYNC_POSITION_S ) )
//  positionSyncV( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRLx_REG_A, SPI_LCD_VSYNC_POSITION_M, SPI_LCD_VSYNC_POSITION_S ) ),
{
}

SpiLCD::~SpiLCD()
{
    delete enable;
    delete blankFrontPorchH;
    delete blankFrontPorchV;
    delete totalH;
    delete totalV;
    delete activeH;
    delete activeV;
    delete idlePolarityV;
    delete idlePolarityH;
    delete widthSyncH;
    delete widthSyncV;
    delete positionSyncH;
//  delete positionSyncV;
}
