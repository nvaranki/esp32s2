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
    positionSyncH( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRL2_REG_A, SPI_LCD_HSYNC_POSITION_M, SPI_LCD_HSYNC_POSITION_S ) ),
//  positionSyncV( new SubValueRW( PeriBus1 + rbo + SPI_LCD_CTRLx_REG_A, SPI_LCD_VSYNC_POSITION_M, SPI_LCD_VSYNC_POSITION_S ) ),
    modeDQS( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_MODE_REG_A, SPI_D_DQS_MODE_M, SPI_D_DQS_MODE_S ) ),
    modeCD( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_MODE_REG_A, SPI_D_CD_MODE_M, SPI_D_CD_MODE_S ) ),
    modeDE( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_MODE_REG_A, SPI_D_DE_MODE_M, SPI_D_DE_MODE_S ) ),
    modeHS( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_MODE_REG_A, SPI_D_HSYNC_MODE_M, SPI_D_HSYNC_MODE_S ) ),
    modeVS( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_MODE_REG_A, SPI_D_VSYNC_MODE_M, SPI_D_VSYNC_MODE_S ) ),
    idlePolarityDE( new FlagRW( PeriBus1 + rbo + SPI_LCD_D_MODE_REG_A, SPI_DE_IDLE_POL_S ) ),
    blankHS( new FlagRW( PeriBus1 + rbo + SPI_LCD_D_MODE_REG_A, SPI_HS_BLANK_EN_S ) ),
    delayDQS( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_NUM_REG_A, SPI_D_DQS_NUM_M, SPI_D_DQS_NUM_S ) ),
    delayCD( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_NUM_REG_A, SPI_D_CD_NUM_M, SPI_D_CD_NUM_S ) ),
    delayDE( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_NUM_REG_A, SPI_D_DE_NUM_M, SPI_D_DE_NUM_S ) ),
    delayHS( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_NUM_REG_A, SPI_D_HSYNC_NUM_M, SPI_D_HSYNC_NUM_S ) ),
    delayVS( new SubValueRW( PeriBus1 + rbo + SPI_LCD_D_NUM_REG_A, SPI_D_VSYNC_NUM_M, SPI_D_VSYNC_NUM_S ) )
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
    delete modeDQS;
    delete modeCD;
    delete modeDE;
    delete modeHS;
    delete modeVS;
    delete idlePolarityDE;
    delete blankHS;
    delete delayDQS;
    delete delayCD;
    delete delayDE;
    delete delayHS;
    delete delayVS;
}
