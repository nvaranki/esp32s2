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
    widthMISO( new BitSetRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_FWRITE_DUAL_M | SPI_FWRITE_QUAD_M | SPI_FWRITE_OCT_M ) ),
    data8(  (uint8_t* )( PeriBus1 + rbo + SPI_W0_REG_A ) ),
    data16( (uint16_t*)( PeriBus1 + rbo + SPI_W0_REG_A ) ),
    data32( (uint32_t*)( PeriBus1 + rbo + SPI_W0_REG_A ) ),
    dataAreaMOSI( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_MOSI_HIGHPART_S ) ),
    dataAreaMISO( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_MISO_HIGHPART_S ) ),
    lengthMOSI( new SubValueRW( PeriBus1 + rbo + SPI_MOSI_DLEN_REG_A, SPI_USR_MOSI_DBITLEN_M, SPI_USR_MOSI_DBITLEN_S ) ),
    lengthMISO( new SubValueRW( PeriBus1 + rbo + SPI_MISO_DLEN_REG_A, SPI_USR_MISO_DBITLEN_M, SPI_USR_MISO_DBITLEN_S ) ),
    delay{ Delay( rbo, 0 ), Delay( rbo, 1 ), Delay( rbo, 2 ), Delay( rbo, 3 ),
           Delay( rbo, 4 ), Delay( rbo, 5 ), Delay( rbo, 6 ), Delay( rbo, 7 ) },
    commandValue(  new SubValueRW( PeriBus1 + rbo + SPI_USER2_REG_A, SPI_USR_COMMAND_VALUE_M,  SPI_USR_COMMAND_VALUE_S  ) ),
    commandLength( new SubValueRW( PeriBus1 + rbo + SPI_USER2_REG_A, SPI_USR_COMMAND_BITLEN_M, SPI_USR_COMMAND_BITLEN_S ) ),
    disableDummyClock( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_DUMMY_IDLE_S ) ),
    phaseCommand( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_COMMAND_S ) ),
    phaseAddress( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_ADDR_S ) ),
    phaseDummy(   new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_DUMMY_S ) ),
    phaseOutput(  new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_MOSI_S ) ),
    phaseInput(   new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_USR_MISO_S ) )
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
    delete dataAreaMOSI;
    delete dataAreaMISO;
    delete lengthMOSI;
    delete lengthMISO;
    delete commandValue;
    delete commandLength;
    delete disableDummyClock;
    delete phaseCommand;
    delete phaseAddress;
    delete phaseDummy;
    delete phaseOutput;
    delete phaseInput;
}

SpiConfig::Delay::Delay( const uint32_t rbo, const uint32_t i ) :
    inp( PeriBus1 + rbo + SPI_DIN_MODE_REG_A,  SPI_DIN0_NUM_V  << ( SPI_DIN0_MODE_S  + 3*i ), SPI_DIN0_MODE_S  + 3*i,
         PeriBus1 + rbo + SPI_DIN_NUM_REG_A,   SPI_DIN0_NUM_V  << ( SPI_DIN0_NUM_S   + 2*i ), SPI_DIN0_NUM_S   + 2*i ),
    out( PeriBus1 + rbo + SPI_DOUT_MODE_REG_A, SPI_DOUT0_NUM_V << ( SPI_DOUT0_MODE_S + 3*i ), SPI_DOUT0_MODE_S + 3*i, 
         PeriBus1 + rbo + SPI_DOUT_NUM_REG_A,  SPI_DOUT0_NUM_V << ( SPI_DOUT0_NUM_S  + 2*i ), SPI_DOUT0_NUM_S  + 2*i )
{
}

SpiConfig::Delay::~Delay()
{
}

SpiConfig::Delay::Descr::Descr( const uint32_t aEdge,  const uint32_t mEdge,  const uint32_t sEdge, 
                                const uint32_t aValue, const uint32_t mValue, const uint32_t sValue ) :
    edge ( new SubValueRW( aEdge,  mEdge,  sEdge  ) ),
    value( new SubValueRW( aValue, mValue, sValue ) )
{
}

SpiConfig::Delay::Descr::~Descr()
{
    delete edge;
    delete value;
}
