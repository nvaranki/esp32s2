// Periphery Unit: multi-channel pulse counter.
//
// © 2021 Nikolai Varankine

#include "soc/pcnt_reg.h"
#include "PulseCountController.hpp"

PulseCountController::PulseCountController() :
    clock( new FlagRW( PCNT_CTRL_REG, PCNT_CLK_EN_S ) ),
    version( new WordRO( PCNT_DATE_REG ) )
{
    for( int i = 0; i < MAX_UNIT; i++ ) ctrl[i] = nullptr;
}
    
PulseCountController::~PulseCountController()
{
    delete clock;
    delete version;
    for( int i = 0; i < MAX_UNIT; i++ ) if( ctrl[i] != nullptr ) delete ctrl[i];
}

PulseCountUnit* PulseCountController::getUnit( const size_t i )
{
    if( i >= MAX_UNIT ) return nullptr;
    if( ctrl[i] == nullptr ) ctrl[i] = new PulseCountUnit( i );
    return ctrl[i];
}
