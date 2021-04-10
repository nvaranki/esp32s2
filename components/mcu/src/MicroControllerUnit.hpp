// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#ifndef H_MicroControllerUnit
#define H_MicroControllerUnit

#include "soc/extmem_reg.h"
#include "PowerManagementUnit.hpp"
#include "io/ControllerIO.hpp"
#include "CoprocessorULP.hpp"
#include "CoreLX7.hpp"
#include "SystemTimer.hpp"
#include "TimerGroup.hpp"
#include "Trigger2.hpp"
#include "bits/BitSetRW.hpp"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

class MicroControllerUnit
{
private:
    CoreLX7* cpu;
    PowerManagementUnit* pmu;
    ControllerIO* io;
    CoprocessorULP* ulp;
    SystemTimer* systemTimer;
    WordRW rr[8]; //TODO WordRW* const rr;
public:
    class Reset
    {
    public:
        /**
         * When set to true, resets the whole digital system, including RTC. 
         * Analog domain stays unaffected.
         */
        FlagWO* const system;
        // /**
        //  * When set to true, resets the whole digital system, 
        //  * including all CPU's, peripherals, Wi-Fi and digital GPIO's. 
        //  * RTC and analog domain stays unaffected.
        //  */
        //TODO FlagWO* const core;
        /**
         * Enable/disable force reset of the digital system (in deep-sleep?!). TODO unclear function
         */
        Trigger2 const digital;
    public:
        Reset();
        virtual ~Reset();
    }
    const reset;
    TimerGroup* const tg0;
    TimerGroup* const tg1;
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
    class ConfigureCache
    {
    public:
        FlagRW* const debug; //!< activate the cache track function,  1: enable  0: disable
    public:
        ConfigureCache();
        virtual ~ConfigureCache();
    }
    const configure;
public:
    CoreLX7* getProcessor( size_t i );
    PowerManagementUnit* getPowerManagementUnit();
    ControllerIO* getControllerIO();
    CoprocessorULP* getCoprocessorULP();
    SystemTimer* getSystemTimer();
    WordRW* getRetentionRegister( size_t i );
    void command( Command c );
};

//taken from #include "esp32s2/ulp.h"
#define RTC_SLOW_MEM ((uint32_t*) 0x50000000)       /*!< RTC slow memory, 8k size */

#endif