// A set of triggers to check pulse counter value. 
// Interrupt control included.
//
// © 2021 Nikolai Varankine

#include "soc/pcnt_reg.h"
#include "PulseCountComparator.hpp"

PulseCountComparator::PulseCountComparator( const size_t unit ) :
    low ( unit, 0 ),
    high( unit, 1 ),
    thr0( unit, 0 ),
    thr1( unit, 1 ),
    zero( unit ),
    interrupt( unit )
{
}
    
PulseCountComparator::~PulseCountComparator()
{
}

PulseCountComparator::LimitComparator::LimitComparator( const size_t unit, bool high ) :
    enable( new FlagRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        high ? PCNT_THR_H_LIM_EN_U0_S : PCNT_THR_L_LIM_EN_U0_S ) ),
    threshold( new SubValueRW( PCNT_U0_CONF2_REG + 0xC * unit, 
        high ? PCNT_CNT_H_LIM_U0_M : PCNT_CNT_L_LIM_U0_M,
        high ? PCNT_CNT_H_LIM_U0_S : PCNT_CNT_L_LIM_U0_S ) ),
    event( new FlagRO( PCNT_U0_STATUS_REG + 0x4 * unit, 
        high ? PCNT_CNT_THR_H_LIM_LAT_U0_S : PCNT_CNT_THR_L_LIM_LAT_U0_S ) )
{
}
    
PulseCountComparator::LimitComparator::~LimitComparator()
{
    delete enable;
    delete threshold;
    delete event;
}

PulseCountComparator::ThresholdComparator::ThresholdComparator( const size_t unit, bool high ) :
    enable( new FlagRW( PCNT_U0_CONF0_REG + 0xC * unit, 
        high ? PCNT_THR_THRES1_EN_U0_S : PCNT_THR_THRES0_EN_U0_S ) ),
    threshold( new SubValueRW( PCNT_U0_CONF1_REG + 0xC * unit, 
        high ? PCNT_CNT_THRES1_U0_M : PCNT_CNT_THRES0_U0_M, 
        high ? PCNT_CNT_THRES1_U0_S : PCNT_CNT_THRES0_U0_S ) ),
    event( new FlagRO( PCNT_U0_STATUS_REG + 0x4 * unit, 
        high ? PCNT_CNT_THR_THRES1_LAT_U0_S : PCNT_CNT_THR_THRES0_LAT_U0_S ) )
{
}
    
PulseCountComparator::ThresholdComparator::~ThresholdComparator()
{
    delete enable;
    delete threshold;
    delete event;
}

PulseCountComparator::ZeroComparator::ZeroComparator( const size_t unit ) :
    enable( new FlagRW( PCNT_U0_CONF0_REG + 0xC * unit, PCNT_THR_ZERO_EN_U0_S ) ),
    // threshold is 0 in hardware
    event( new FlagRO( PCNT_U0_STATUS_REG + 0x4 * unit, PCNT_CNT_THR_ZERO_LAT_U0_S ) )
{
}
    
PulseCountComparator::ZeroComparator::~ZeroComparator()
{
    delete enable;
}

PulseCountComparator::Interrupt::Interrupt( const size_t unit ) :
    InterruptController( 
        new FlagRO( PCNT_INT_RAW_REG, PCNT_CNT_THR_EVENT_U0_INT_RAW_S + unit ),
        new FlagRO( PCNT_INT_ST_REG,  PCNT_CNT_THR_EVENT_U0_INT_ST_S  + unit ),
        new FlagRW( PCNT_INT_ENA_REG, PCNT_CNT_THR_EVENT_U0_INT_ENA_S + unit ),
        new FlagWO( PCNT_INT_CLR_REG, PCNT_CNT_THR_EVENT_U0_INT_CLR_S + unit ) )
{
}
    
PulseCountComparator::Interrupt::~Interrupt()
{
    delete raw;
    delete masked;
    delete enable;
    delete clear;
}
