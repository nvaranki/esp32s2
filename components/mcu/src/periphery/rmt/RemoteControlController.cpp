// Periphery Unit: multi-channel configurable pulse I/O for remote control.
//
// © 2021 Nikolai Varankine

#include "soc/rmt_reg.h"
#include "soc/system_reg.h"
#include "RemoteControlController.hpp"

RemoteControlController::RemoteControlController() :
    clock( new FlagRW( DPORT_PERIP_CLK_EN0_REG, DPORT_RMT_CLK_EN_S ) ),
    reset( new FlagRW( DPORT_PERIP_RST_EN0_REG, DPORT_RMT_RST_S ) ),
    enable( new FlagRW( RMT_APB_CONF_REG, RMT_CLK_EN_S ) ),
    sendAltogether( new FlagRW( RMT_TX_SIM_REG, RMT_TX_SIM_EN_S ) ),
    memory( new RemoteControlMemory() ),
    version( new WordRO( RMT_DATE_REG ) )
{
    for( int i = 0; i < MAX_CHANNEL; i++ ) cnl[i] = nullptr;
}

RemoteControlController::~RemoteControlController()
{
    delete clock;
    delete reset;
    delete enable;
    delete sendAltogether;
    delete memory;
    delete version;
    for( int i = 0; i < MAX_CHANNEL; i++ ) if( cnl[i] != nullptr ) delete cnl[i];
}

RemoteControlChannel* RemoteControlController::getChannel( const uint32_t i )
{
    if( i >= MAX_CHANNEL ) return nullptr;
    if( cnl[i] == nullptr ) cnl[i] = new RemoteControlChannel( i );
    return cnl[i];
}
