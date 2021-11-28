#include "SerialPeripheralInterface.hpp"
#include "SpiController.hpp"

SerialPeripheralInterface::SerialPeripheralInterface()
{
    for( int i = 0; i < MAX_UNIT; i++ ) ctrl[i] = nullptr;
}

SerialPeripheralInterface::~SerialPeripheralInterface()
{
    for( int i = 0; i < MAX_UNIT; i++ ) if( ctrl[i] != nullptr ) delete ctrl[i];
}

SpiController* SerialPeripheralInterface::getUnit( const uint32_t i )
{
    if( i >= MAX_UNIT ) return nullptr;
    if( ctrl[i] == nullptr ) 
        ctrl[i] = new SpiController( i, BaseAddress[i] );
    return ctrl[i];
}
