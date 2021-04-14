// Input pulse channel.
//
// © 2021 Nikolai Varankine

#include "soc/pcnt_reg.h"
#include "PulseCountChannel.hpp"
#include "PulseCountUnit.hpp"

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
    unit( unit ),
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

size_t PulseCountChannel::getMatrixIndex( const Input i ) const 
{ 
    // See Table 22: GPIO Matrix
    // 39 pcnt_sig_ch0_in0 
    // 40 pcnt_sig_ch1_in0
    // 41 pcnt_ctrl_ch0_in0
    // 42 pcnt_ctrl_ch1_in0
    // ... and so on
    return ( i == Input::SIGNAL ? PCNT_SIG_CH0_IN0_IDX : PCNT_CTRL_CH0_IN0_IDX ) 
        + unit * ( PulseCountUnit::MAX_CHANNEL * 2 /* item count in Input */ ) 
        + id; 
}
