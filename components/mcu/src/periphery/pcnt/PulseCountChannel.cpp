// Input pulse channel.
//
// For peripheral input index see Table 22: GPIO Matrix 
// 39 pcnt_sig_ch0_in0 
// 40 pcnt_sig_ch1_in0
// 41 pcnt_ctrl_ch0_in0
// 42 pcnt_ctrl_ch1_in0
// ... and so on
//
// © 2021 Nikolai Varankine

#include "soc/pcnt_reg.h"
#include "PulseCountChannel.hpp"
#include "PulseCountUnit.hpp"

PulseCountChannel::PulseCountChannel( const size_t unit, const size_t channel ) :
    signal ( unit, channel ),
    control( unit, channel ),
    unit( unit ),
    id( channel )
{
}
    
PulseCountChannel::~PulseCountChannel()
{
}

PulseCountChannel::Signal::Signal( const size_t unit, const size_t channel ) :
    negative( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        channel ? PCNT_CH1_NEG_MODE_U0_M : PCNT_CH0_NEG_MODE_U0_M, 
        channel ? PCNT_CH1_NEG_MODE_U0_S : PCNT_CH0_NEG_MODE_U0_S ) ),
    positive( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        channel ? PCNT_CH1_POS_MODE_U0_M : PCNT_CH0_POS_MODE_U0_M, 
        channel ? PCNT_CH1_POS_MODE_U0_S : PCNT_CH0_POS_MODE_U0_S ) ),
    index( PCNT_SIG_CH0_IN0_IDX + channel
        + unit * ( PulseCountUnit::MAX_CHANNEL * 2 /* signal + control */ ) )
{
}
    
PulseCountChannel::Signal::~Signal()
{
    delete negative;
    delete positive;
}

PulseCountChannel::Control::Control( const size_t unit, const size_t channel ) :
    high( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        channel ? PCNT_CH1_HCTRL_MODE_U0_M : PCNT_CH0_HCTRL_MODE_U0_M, 
        channel ? PCNT_CH1_HCTRL_MODE_U0_S : PCNT_CH0_HCTRL_MODE_U0_S ) ),
    low ( new SubValueRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        channel ? PCNT_CH1_LCTRL_MODE_U0_M : PCNT_CH0_LCTRL_MODE_U0_M, 
        channel ? PCNT_CH1_LCTRL_MODE_U0_S : PCNT_CH0_LCTRL_MODE_U0_S ) ),
    index( PCNT_CTRL_CH0_IN0_IDX + channel
        + unit * ( PulseCountUnit::MAX_CHANNEL * 2 /* signal + control */ ) )
{
}
    
PulseCountChannel::Control::~Control()
{
    delete high;
    delete low;
}
