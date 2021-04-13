// Input pulse channel.
//
// © 2021 Nikolai Varankine

#include "soc/pcnt_reg.h"
#include "PulseCountChannel.hpp"

PulseCountChannel::PulseCountChannel( const size_t unit, const size_t i ) :
    negative( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        i ? PCNT_CH1_NEG_MODE_U0_M : PCNT_CH0_NEG_MODE_U0_M, 
        i ? PCNT_CH1_NEG_MODE_U0_S : PCNT_CH0_NEG_MODE_U0_S ) ),
    positive( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        i ? PCNT_CH1_POS_MODE_U0_M : PCNT_CH0_POS_MODE_U0_M, 
        i ? PCNT_CH1_POS_MODE_U0_S : PCNT_CH0_POS_MODE_U0_S ) ),
    high    ( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        i ? PCNT_CH1_HCTRL_MODE_U0_M : PCNT_CH0_HCTRL_MODE_U0_M, 
        i ? PCNT_CH1_HCTRL_MODE_U0_S : PCNT_CH0_HCTRL_MODE_U0_S ) ),
    low     ( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        i ? PCNT_CH1_LCTRL_MODE_U0_M : PCNT_CH0_LCTRL_MODE_U0_M, 
        i ? PCNT_CH1_LCTRL_MODE_U0_S : PCNT_CH0_LCTRL_MODE_U0_S ) ),
    id( i )
{
}
    
PulseCountChannel::~PulseCountChannel()
{
    delete negative;
    delete positive;
    delete high;
    delete low;
}
