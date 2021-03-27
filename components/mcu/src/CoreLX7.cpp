// Main Xtensa® LX7 processor core.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "CoreLX7.hpp"

CoreLX7::CoreLX7( const int id ) :
    staticVector( new FlagRW( RTC_CNTL_RESET_STATE_REG, RTC_CNTL_PROCPU_STAT_VECTOR_SEL_S ) ),
//  staticVector( new FlagRW( RTC_CNTL_RESET_STATE_REG, RTC_CNTL_APPCPU_STAT_VECTOR_SEL_S ) ),
    reset( new FlagWO( RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_PROCPU_RST_S ) ),
//  reset( new FlagWO( RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_APPCPU_RST_S ) ),
    resetCause( new SubValueRO( RTC_CNTL_RESET_STATE_REG, RTC_CNTL_RESET_CAUSE_PROCPU_M, RTC_CNTL_RESET_CAUSE_PROCPU_S ) ),
//  resetCause( new SubValueRO( RTC_CNTL_RESET_STATE_REG, RTC_CNTL_RESET_CAUSE_APPCPU_M, RTC_CNTL_RESET_CAUSE_APPCPU_S ) ),
    control( new SubValueRW( RTC_CNTL_SW_CPU_STALL_REG, RTC_CNTL_SW_STALL_PROCPU_C1_M, RTC_CNTL_SW_STALL_PROCPU_C1_S ) ),
//  control( new SubValueRW( RTC_CNTL_SW_CPU_STALL_REG, RTC_CNTL_SW_STALL_APPCPU_C1_M, RTC_CNTL_SW_STALL_APPCPU_C1_S ) ),
    id( id )
{
}

CoreLX7::~CoreLX7()
{
    delete staticVector;
    delete resetCause;
    delete control;
    delete reset;
}

CoreLX7::ResetCause CoreLX7::getResetCause()
{
    ResetCause rc;
    switch( resetCause->get() ) //TODO map?
    {
    case static_cast<uint32_t>( ResetCause::CHIP   ) : rc = ResetCause::CHIP;   break;
    case static_cast<uint32_t>( ResetCause::SYSTEM ) : rc = ResetCause::SYSTEM; break;
    case static_cast<uint32_t>( ResetCause::SLEEP  ) : rc = ResetCause::SLEEP;  break;
    case static_cast<uint32_t>( ResetCause::MWDT0G ) : rc = ResetCause::MWDT0G; break;
    case static_cast<uint32_t>( ResetCause::MWDT1G ) : rc = ResetCause::MWDT1G; break;
    case static_cast<uint32_t>( ResetCause::RWDTC  ) : rc = ResetCause::RWDTC;  break;
    case static_cast<uint32_t>( ResetCause::MWDT0  ) : rc = ResetCause::MWDT0;  break;
    case static_cast<uint32_t>( ResetCause::SOFT   ) : rc = ResetCause::SOFT;   break;
    case static_cast<uint32_t>( ResetCause::RWDTP  ) : rc = ResetCause::RWDTP;  break;
    case static_cast<uint32_t>( ResetCause::BROWN  ) : rc = ResetCause::BROWN;  break;
    case static_cast<uint32_t>( ResetCause::RWDTS  ) : rc = ResetCause::RWDTS;  break;
    case static_cast<uint32_t>( ResetCause::MWDT1  ) : rc = ResetCause::MWDT1;  break;
    case static_cast<uint32_t>( ResetCause::GLITCH ) : rc = ResetCause::GLITCH; break;
    default: rc = ResetCause::UNSET; break;
    }
    return rc;
}

void CoreLX7::command( CoreLX7::Command c )
{
    control->set( static_cast<uint32_t>( c ) );
}

void CoreLX7::setBootVectorRTC( CoreLX7::BootVectorRTC v ) 
{ 
    staticVector->set( static_cast<bool>( v ) ); 
}
    
CoreLX7::BootVectorRTC CoreLX7::getBootVectorRTC() const 
{ 
    return staticVector->get() ? BootVectorRTC::FAST : BootVectorRTC::SLOW; 
}
