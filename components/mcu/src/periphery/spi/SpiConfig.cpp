#include "SpiConfig.hpp"

#include "MicroControllerUnit.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiConfig::SpiConfig( const uint32_t rbo ) :
    configLength( new SubValueRW( PeriBus1 + rbo + SPI_CMD_REG_A, SPI_CONF_BITLEN_M, SPI_CONF_BITLEN_S ) ),
    fullDuplex( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_DOUTDIN_S ) ),
    modeQPI( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_QPI_MODE_S ) ),
    modeOPI( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_OPI_MODE_S ) ),
    holdCS( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CS_HOLD_S ) ),
    setupCS( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CS_SETUP_S ) ),
    delayMOSI( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_CK_OUT_EDGE_S ) ),
    bitOrderMISO( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_RD_BYTE_ORDER_S ) ),
    bitOrderMOSI( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_WR_BYTE_ORDER_S ) ),
    widthMISO( new BitSetRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_FWRITE_DUAL_M | SPI_FWRITE_QUAD_M | SPI_FWRITE_OCT_M ) )
{
}

SpiConfig::~SpiConfig()
{
    delete configLength;
    delete fullDuplex;
    delete modeQPI;
    delete modeOPI;
    delete holdCS;
    delete setupCS;
    delete delayMOSI;
    delete bitOrderMISO;
    delete bitOrderMOSI;
    delete widthMISO;
}
