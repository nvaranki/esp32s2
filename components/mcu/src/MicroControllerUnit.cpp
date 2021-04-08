// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#include "MicroControllerUnit.hpp"

MicroControllerUnit::MicroControllerUnit() :
    cpu( nullptr ), pmu( nullptr ), io( nullptr ),  ulp( nullptr ), 
    /** Registers that always are powered up */
    rr{ // usage info from "esp32s2/rom/rtc.h"
        WordRW( RTC_CNTL_STORE0_REG ), // Reserved
        WordRW( RTC_CNTL_STORE1_REG ), // RTC_SLOW_CLK calibration value
        WordRW( RTC_CNTL_STORE2_REG ), // Boot time, low word
        WordRW( RTC_CNTL_STORE3_REG ), // Boot time, high word
        WordRW( RTC_CNTL_STORE4_REG ), // External XTAL frequency
        WordRW( RTC_CNTL_STORE5_REG ), // APB bus frequency
        WordRW( RTC_CNTL_STORE6_REG ), // FAST_RTC_MEMORY_ENTRY TODO ?! RTC fast memory boot: CRC
        WordRW( RTC_CNTL_STORE7_REG ), // FAST_RTC_MEMORY_CRC   TODO ?! RTC fast memory boot: entry address within fast memory 
        },
    resetSystem( new FlagWO( RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_SYS_RST_S ) ),
    tg0( new TimerGroup( 0 ) ),
    tg1( new TimerGroup( 1 ) ),
    control( new SubValueRW( RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_STALL_PROCPU_C0_M, RTC_CNTL_SW_STALL_PROCPU_C0_S ) )
//  control( new SubValueRW( RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_STALL_APPCPU_C0_M, RTC_CNTL_SW_STALL_APPCPU_C0_S ) )
{
}

MicroControllerUnit::~MicroControllerUnit()
{
    if( pmu != nullptr ) delete pmu;
    if( io  != nullptr ) delete io;
    if( ulp != nullptr ) delete ulp;
    delete resetSystem;
    delete tg0;
    delete tg1;
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

ControllerIO* MicroControllerUnit::getControllerIO()
{
    if( io == nullptr )
        io = new ControllerIO();
    return io;
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
