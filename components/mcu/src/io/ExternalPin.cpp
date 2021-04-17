// Controller of GPIO pin.
//
// © 2021 Nikolai Varankine

#include "ExternalPin.hpp"

#define GPIO_REG(i) ( PERIPHS_IO_MUX_GPIO0_U + i * 0x4u )

ExternalPin::ExternalPin( const size_t i ) :
    sleep( i ),
    strength( new SubValueRW( GPIO_REG(i), FUN_DRV_M, FUN_DRV_S ) ),
    pull( new BitSetRW( GPIO_REG(i), FUN_PD_M | FUN_PU_M ) ),
    sync1( new SubValueRW( GPIO_PIN0_REG + i * 0x4u, GPIO_PIN0_SYNC1_BYPASS_M, GPIO_PIN0_SYNC1_BYPASS_S ) ),
    sync2( new SubValueRW( GPIO_PIN0_REG + i * 0x4u, GPIO_PIN0_SYNC2_BYPASS_M, GPIO_PIN0_SYNC2_BYPASS_S ) ),
    input( new FlagRW( GPIO_REG(i), FUN_IE_S ) ),
    function( new SubValueRW( GPIO_REG(i), MCU_SEL_M, MCU_SEL_S ) ),
    filter( new FlagRW( GPIO_REG(i), 15 ) ), //TODO mnemonics, undefined in ESP-IDF
    number( i )
{
}
    
ExternalPin::~ExternalPin()
{
    delete strength;
    delete pull;
    delete sync1;
    delete sync2;
    delete input;
    delete function;
    delete filter;
}

ExternalPin::Pull ExternalPin::getPull() const
{ 
    return pull->get( FUN_PD_M ) ? Pull::DOWN : pull->get( FUN_PU_M ) ? Pull::UP : Pull::OPEN;
}

ExternalPin::DriveStrength ExternalPin::getDriveStrength() const 
{ 
    return (DriveStrength) strength->get(); 
}

void ExternalPin::setDriveStrength( DriveStrength v ) 
{ 
    strength->set( static_cast<uint32_t>( v ) ); 
}
    
void ExternalPin::setPull( ExternalPin::Pull v )
{
    pull->setAll( false ); // go Pull::OPEN, safety step for other modes
    uint32_t m = static_cast<uint32_t>( v );
    if( m )
        pull->set( m, true );
}

ExternalPin::SyncMode ExternalPin::getSync( const SyncStage stage ) const 
{ 
    return (SyncMode) ( stage == SyncStage::FIRST ? sync1 : sync2 )->get(); 
}

void ExternalPin::setSync( const SyncStage stage, const SyncMode mode ) 
{ 
    ( stage == SyncStage::FIRST ? sync1 : sync2 )->set( static_cast<uint32_t>( mode ) ); 
}

ExternalPin::Sleep::Sleep( const size_t i ) :
    pull  ( new BitSetRW( GPIO_REG(i), SLP_PD_M | SLP_PU_M ) ),
    enable( new FlagRW( GPIO_REG(i), SLP_SEL_S ) ),
    input ( new FlagRW( GPIO_REG(i), SLP_IE_S ) ),
    output( new FlagRW( GPIO_REG(i), SLP_OE_S ) )
{
}
    
ExternalPin::Sleep::~Sleep()
{
    delete pull;
    delete enable;
    delete input;
    delete output;
}

ExternalPin::Sleep::Pull ExternalPin::Sleep::getPull() const
{ 
    return pull->get( SLP_PD_M ) ? Pull::DOWN : pull->get( SLP_PU_M ) ? Pull::UP : Pull::OPEN;
}
    
void ExternalPin::Sleep::setPull( ExternalPin::Sleep::Pull v )
{
    pull->setAll( false ); // go Pull::OPEN, safety step for other modes
    uint32_t m = static_cast<uint32_t>( v );
    if( m )
        pull->set( m, true );
}

#undef GPIO_REG
