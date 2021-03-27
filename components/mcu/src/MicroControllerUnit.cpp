// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#include "MicroControllerUnit.hpp"

MicroControllerUnit::MicroControllerUnit() :
    cpu( nullptr ), pmu( nullptr ), ulp( nullptr ), 
    /** Registers that always are powered up */
    rr{ 
        WordRW( RTC_CNTL_STORE0_REG ), 
        WordRW( RTC_CNTL_STORE1_REG ), 
        WordRW( RTC_CNTL_STORE2_REG ), 
        WordRW( RTC_CNTL_STORE3_REG ), 
        WordRW( RTC_CNTL_STORE4_REG ), 
        WordRW( RTC_CNTL_STORE5_REG ), 
        WordRW( RTC_CNTL_STORE6_REG ), // RTC fast memory boot: CRC
        WordRW( RTC_CNTL_STORE7_REG ), // RTC fast memory boot: entry address within fast memory 
        },
    resetSystem( new FlagWO( RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_SYS_RST_S ) ),
    control( new SubValueRW( RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_STALL_PROCPU_C0_M, RTC_CNTL_SW_STALL_PROCPU_C0_S ) )
//  control( new SubValueRW( RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_STALL_APPCPU_C0_M, RTC_CNTL_SW_STALL_APPCPU_C0_S ) )
{
}

MicroControllerUnit::~MicroControllerUnit()
{
    if( pmu != nullptr ) delete pmu;
    if( ulp != nullptr ) delete ulp;
    delete resetSystem;
    delete control;
}

CoreLX7* MicroControllerUnit::getProcessor( size_t i )
{
    if( i != 0 )
        return nullptr;
    if( cpu == nullptr )
        cpu = new CoreLX7( 0 );
    return cpu;
}

PowerManagementUnit* MicroControllerUnit::getPowerManagementUnit()
{
    if( pmu == nullptr )
        pmu = new PowerManagementUnit();
    return pmu;
}

CoprocessorULP* MicroControllerUnit::getCoprocessorULP()
{
    if( ulp == nullptr )
        ulp = new CoprocessorULP();
    return ulp;
}

WordRW* MicroControllerUnit::getRetentionRegister( size_t i ) 
{ 
    return i < 8 ? rr + i : nullptr;
}

void MicroControllerUnit::stop()
{
    //TODO not impl.
}

void MicroControllerUnit::command( Command c )
{
    control->set( static_cast<uint32_t>( c ) );
}

MicroControllerUnit::ConfigureCache::ConfigureCache() :
    debug( new FlagRW( EXTMEM_CACHE_DBG_INT_ENA_REG, EXTMEM_CACHE_DBG_EN_S ) )
{
}
    
MicroControllerUnit::ConfigureCache::~ConfigureCache()
{
    delete debug;
}
