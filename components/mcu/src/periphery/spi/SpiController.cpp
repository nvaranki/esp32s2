#include "SerialPeripheralInterface.hpp"
#include "MicroControllerUnit.hpp"
#include "SpiController.hpp"
#include "SpiRegisters.hpp"
#include "soc/spi_reg.h"

#define PeriBus1 (uint32_t) MicroControllerUnit::Bus::PeriBus1
#define PeriBus2 (uint32_t) MicroControllerUnit::Bus::PeriBus2

SpiController::SpiController( const uint32_t i, const uint32_t rbo ) :
    id( i ),
    userDefinedCommand( new FlagRW( PeriBus1 + rbo + SPI_CMD_REG_A, SPI_USR_S ) ),
    phase( rbo ),
    dma( new SpiDMA( rbo ) ),
    lcd( new SpiLCD( rbo ) ),
    slave( new FlagRW( PeriBus1 + rbo + SPI_SLAVE0_REG_A, SPI_SLAVE_MODE_S ) ),
    fullDuplex( new FlagRW( PeriBus1 + rbo + SPI_USER0_REG_A, SPI_DOUTDIN_S ) ),
    version( new SubValueRW( PeriBus1 + rbo + SPI_DATE_REG_A, SPI_DATE_M, SPI_DATE_S ) )
{
}

SpiController::~SpiController()
{
    delete userDefinedCommand;
    delete dma;
    delete lcd;
    delete slave;
    delete fullDuplex;
    delete version;
}
    
SpiController::Phase::Phase( const uint32_t rbo ) :
    idle( new SpiIdle( rbo ) ),
    config( new SpiConfig( rbo ) ),
    prepare( new SpiPrepare( rbo ) ),
    command( new SpiCommand( rbo ) ),
    address( new SpiAddress( rbo ) ),
    dummy( new SpiDummy( rbo ) ),
    output( new SpiOutput( rbo ) ),
    input( new SpiInput( rbo ) ),
    done( new SpiDone( rbo ) )
{
}
    
SpiController::Phase::~Phase()
{
    delete idle;
    delete config;
    delete prepare;
    delete command;
    delete address;
    delete dummy;
    delete output;
    delete input;
    delete done;
}
