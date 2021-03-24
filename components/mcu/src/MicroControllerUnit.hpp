// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#ifndef H_MicroControllerUnit
#define H_MicroControllerUnit

#include "PowerManagementUnit.hpp"
#include "CoprocessorULP.hpp"
#include "CoreLX7.hpp"
#include "bits/BitSetRW.hpp"
#include "bits/WordRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"

class MicroControllerUnit
{
private:
    CoreLX7* cpu;
    PowerManagementUnit* pmu;
    CoprocessorULP* ulp;
    WordRW rr[8]; //TODO WordRW* const rr;
public:
    /**
     * When set to true, resets the whole digital system, including RTC. 
     * Analog domain stays unaffected.
     */
    FlagWO* const resetSystem;
    // /**
    //  * When set to true, resets the whole digital system, 
    //  * including all CPU's, peripherals, Wi-Fi and digital GPIO's. 
    //  * RTC and analog domain stays unaffected.
    //  */
    //TODO FlagWO* const resetCore;
private:
    SubValueRW* const control;
public:
    MicroControllerUnit();
    virtual ~MicroControllerUnit();
public:
    enum class Command : uint32_t
    {
        CLEAR = 0x0,
        STALL = 0x2, //!< setting this bit stalls the CPU Core, see CoreLX7::Command::STALL
    };
public:
    CoreLX7* getProcessor( size_t i );
    PowerManagementUnit* getPowerManagementUnit();
    CoprocessorULP* getCoprocessorULP();
    WordRW* getRetentionRegister( size_t i );
    void command( Command c );
};

//taken from #include "esp32s2/ulp.h"
#define RTC_SLOW_MEM ((uint32_t*) 0x50000000)       /*!< RTC slow memory, 8k size */

#endif