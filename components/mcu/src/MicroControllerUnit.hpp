// ESP32-S2 Microcontroller unit.
//
// © 2021 Nikolai Varankine

#ifndef H_MicroControllerUnit
#define H_MicroControllerUnit

#include "soc/extmem_reg.h"
#include "PowerManagementUnit.hpp"
#include "io/ControllerIO.hpp"
#include "periphery/pcnt/PulseCountController.hpp"
#include "periphery/rmt/RemoteControlController.hpp"
#include "periphery/spi/SerialPeripheralInterface.hpp"
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
    /** clock selector for CPU, etc. */
    SubValueRW* const clockSoC;
    /** Registers that always are powered up */
    WordRW rr[8]; //TODO WordRW* const rr;
public:
    /** 0:320 / 1:480 MHz selector */
    FlagRW* const pllFreqHigh;
    /** PLL/APLL clock frequency upraiser for CPU, etc. 
     * 0: PLL/6, APLL/4
     * 1: PLL/3, APLL/2
     * 2: PLL/2, -
    */
    SubValueRW* const upraiseClockSoC;
public:
    class Periphery
    {
    private:
        PulseCountController* pcnt;
        RemoteControlController* rmt;
        SerialPeripheralInterface* spi;
    public:
        Periphery();
        virtual ~Periphery();
    public:
        PulseCountController* getPulseCountController();
        RemoteControlController* getRemoteControlController();
        SerialPeripheralInterface* getSerialPeripheralInterface();
    }
    /*const*/ periphery;
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
    enum class Bus : uint32_t
    {
        PeriBus1 = 0x3F400000, //!< Refers to the address range 0x3F40_0000 ~ 0x3F4F_FFFF. 0x3F40_0000 is the base address.
        PeriBus2 = 0x60000000, //!< Refers to the address ranges 0x6000_0000 ~ 0x600B_FFFF and 0x6180_0000 ~ 0x6180_3FFF on the bus. 0x6000_0000 is the base address.

    };
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
    enum class ClockSoC : uint32_t //!< clock for CPU, etc.
    {
        XTAL  = DPORT_SOC_CLK_SEL_XTL, //!< 
        PLL   = DPORT_SOC_CLK_SEL_PLL, //!< 
        RTC8M = DPORT_SOC_CLK_SEL_8M, //!< 
        APLL  = DPORT_SOC_CLK_SEL_APLL, //!< 
    };
    inline ClockSoC getClockSoC() const { return (ClockSoC) clockSoC->get(); }
    inline void setClockSoC( const ClockSoC v ) { clockSoC->set( static_cast<uint32_t>( v ) ); }
};

//taken from #include "esp32s2/ulp.h"
#define RTC_SLOW_MEM ((uint32_t*) 0x50000000)       /*!< RTC slow memory, 8k size */

#endif
