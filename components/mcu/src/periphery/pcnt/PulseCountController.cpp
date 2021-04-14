// Periphery Unit: multi-channel pulse counter.
//
// © 2021 Nikolai Varankine

#include "soc/pcnt_reg.h"
#include "soc/system_reg.h"
#include "PulseCountController.hpp"

PulseCountController::PulseCountController() :
    clock( new FlagRW( DPORT_PERIP_CLK_EN0_REG, DPORT_PCNT_CLK_EN_S ) ),
    reset( new FlagRW( DPORT_PERIP_RST_EN0_REG, DPORT_PCNT_RST_S ) ),
    enable( new FlagRW( PCNT_CTRL_REG, PCNT_CLK_EN_S ) ),
    version( new WordRO( PCNT_DATE_REG ) )
{
    for( int i = 0; i < MAX_UNIT; i++ ) ctrl[i] = nullptr;
}
    
PulseCountController::~PulseCountController()
{
    delete clock;
    delete reset;
    delete enable;
    delete version;
    for( int i = 0; i < MAX_UNIT; i++ ) if( ctrl[i] != nullptr ) delete ctrl[i];
}

PulseCountUnit* PulseCountController::getUnit( const size_t i )
{
    if( i >= MAX_UNIT ) return nullptr;
    if( ctrl[i] == nullptr ) ctrl[i] = new PulseCountUnit( i );
    return ctrl[i];
}
