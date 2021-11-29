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
    conf( new SpiConfig( rbo ) ),
    prep( new SpiPrepare( rbo ) ),
    done( new SpiDone( rbo ) ),
    dma( new SpiDMA( rbo ) ),
    lcd( new SpiLCD( rbo ) )
{
}

SpiController::~SpiController()
{
    delete userDefinedCommand;
    delete conf;
    delete prep;
    delete done;
    delete dma;
    delete lcd;
}
