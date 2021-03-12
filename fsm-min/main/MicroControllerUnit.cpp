// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#include "MicroControllerUnit.hpp"

MicroControllerUnit::MicroControllerUnit() :
    pmu( nullptr ), ulp( nullptr )
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

void MicroControllerUnit::stop()
{
    //TODO not impl.
}
