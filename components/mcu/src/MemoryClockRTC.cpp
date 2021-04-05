// Low power clocks controller for RTC slow and fast memory.
//
// © 2021 Nikolai Varankine

#include "MemoryClockRTC.hpp"

MemoryClockRTC::MemoryClockRTC() :
    src( new SubValueRW( DPORT_BT_LPCK_DIV_FRAC_REG, 
        DPORT_LPCLK_SEL_RTC_SLOW_M | DPORT_LPCLK_SEL_8M_M | DPORT_LPCLK_SEL_XTAL_M | DPORT_LPCLK_SEL_XTAL32K_M, 
        DPORT_LPCLK_SEL_RTC_SLOW_S ) ),
    enable( new FlagRW( DPORT_BT_LPCK_DIV_FRAC_REG, DPORT_LPCLK_RTC_EN_S ) ),
    divider ( new SubValueRW( DPORT_BT_LPCK_DIV_INT_REG,  DPORT_BT_LPCK_DIV_NUM_M, DPORT_BT_LPCK_DIV_NUM_S ) ),
    dividerA( new SubValueRW( DPORT_BT_LPCK_DIV_FRAC_REG, DPORT_BT_LPCK_DIV_A_M,   DPORT_BT_LPCK_DIV_A_S   ) ),
    dividerB( new SubValueRW( DPORT_BT_LPCK_DIV_FRAC_REG, DPORT_BT_LPCK_DIV_B_M,   DPORT_BT_LPCK_DIV_B_S   ) )
{
}

MemoryClockRTC::~MemoryClockRTC() 
{
    delete src;
    delete enable;
    delete divider;
    delete dividerA;
    delete dividerB;
}

MemoryClockRTC::Source MemoryClockRTC::getSource() const 
{ 
    return (Source) src->get(); 
}

void MemoryClockRTC::setSource( const MemoryClockRTC::Source value ) 
{ 
    src->set( static_cast<uint32_t>( value ) ); 
}
