// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#ifndef H_MicroControllerUnit
#define H_MicroControllerUnit

#include "PowerManagementUnit.hpp"
#include "CoprocessorULP.hpp"

class MicroControllerUnit
{
private:
    PowerManagementUnit* pmu;
    CoprocessorULP* ulp;
public:
    MicroControllerUnit();
    virtual ~MicroControllerUnit();
public:
    PowerManagementUnit* getPowerManagementUnit();
    CoprocessorULP* getCoprocessorULP();
    void stop();
};

#endif