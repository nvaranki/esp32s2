#include "MicroControllerUnit.hpp"

MicroControllerUnit::MicroControllerUnit(/* args */)
{
}

MicroControllerUnit::~MicroControllerUnit()
{
    if( pmu != nullptr )
        delete pmu;
}

PowerManagementUnit* MicroControllerUnit::getPowerManagementUnit()
{
    if( pmu == nullptr )
        pmu = new PowerManagementUnit();
    return pmu;
}