// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#ifndef H_MicroControllerUnit
#define H_MicroControllerUnit

#include "PowerManagementUnit.hpp"
#include "CoprocessorULP.hpp"
#include "bits/WordRW.hpp"

class MicroControllerUnit
{
private:
    PowerManagementUnit* pmu;
    CoprocessorULP* ulp;
    WordRW rr[8]; //TODO WordRW* const rr;
public:
    MicroControllerUnit();
    virtual ~MicroControllerUnit();
public:
    PowerManagementUnit* getPowerManagementUnit();
    CoprocessorULP* getCoprocessorULP();
    WordRW* getRetentionRegister( size_t i );
    void stop();
};

//taken from #include "esp32s2/ulp.h"
#define RTC_SLOW_MEM ((uint32_t*) 0x50000000)       /*!< RTC slow memory, 8k size */

#endif