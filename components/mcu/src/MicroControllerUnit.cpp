// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#include "MicroControllerUnit.hpp"

MicroControllerUnit::MicroControllerUnit() :
    pmu( nullptr ), ulp( nullptr ), 
    rr{ 
        WordRW( RTC_CNTL_STORE0_REG ), 
        WordRW( RTC_CNTL_STORE1_REG ), 
        WordRW( RTC_CNTL_STORE2_REG ), 
        WordRW( RTC_CNTL_STORE3_REG ), 
        WordRW( RTC_CNTL_STORE4_REG ), 
        WordRW( RTC_CNTL_STORE5_REG ), 
        WordRW( RTC_CNTL_STORE6_REG ), 
        WordRW( RTC_CNTL_STORE7_REG ) }
{
}

MicroControllerUnit::~MicroControllerUnit()
{
    if( pmu != nullptr ) delete pmu;
    if( ulp != nullptr ) delete ulp;
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
    return rr + i; //TODO index check
}

void MicroControllerUnit::stop()
{
    //TODO not impl.
}
