// Ultra low power (ULP) co-processor.
//
// © 2021 Nikolai Varankine

#ifndef H_CoprocessorULP
#define H_CoprocessorULP

#include "esp_err.h"
#include "soc/rtc_cntl_reg.h"
#include "CoreFSM.hpp"
#include "CoreRISCV.hpp"
#include "bits/BitSetRW.hpp"
#include "bits/SubValueRW.hpp"
#include "TimerULP.hpp"

class CoprocessorULP 
{
private:
    CoreFSM* const fsm;
    CoreRISCV* const riscv;
    TimerULP* const timer;
//  BitSetRW* const cfgCore; //!< configuration register
    BitSetRW* const cfgGPIO; //!< configuration register
public:
    class Status
    {
    public:
        /**
         * When found true, indicates a sleep state of ULP FSM/RISCV. Usually this is an
         * inverse value of "cocpuWork" property.
         * TODO undocumented
         */
        FlagRO* const sleep;
        /**
         * When found true, indicates a working state of ULP FSM/RISCV. Usually this is an
         * inverse value of "cocpuSleep" property.
         * TODO undocumented
         */
        FlagRO* const work;
        /**
         * When found true, indicates a state of ULP FSM/RISCV when it runs a program. Usually this is an
         * inverse value of "cocpuDone" property.
         * TODO undocumented
         */
        FlagRO* const runs;
        /**
         * When found true, indicates a state of ULP FSM/RISCV when it finished a program. Usually this is an
         * inverse value of "cocpuRuns" property.
         * TODO undocumented
         */
        FlagRO* const done; //!< ulp/cocpu is done TODO undocumented
    public:
        Status();
        virtual ~Status();
    }
    const status;
    /**
     * Relative, to RTC_SLOW_MEM, an address of first program instruction,
     * expressed in 32 bit words.
     */
    SubValueRW* const entry;
public:
    CoprocessorULP();
    virtual ~CoprocessorULP();
public:
    enum class Core : bool
    {
        RISCV = false,
        FSM   = true,
    };
    enum class ConfigGPIO : uint32_t
    {
        // RTC_CNTL_ULP_CP_TIMER_REG (0x00F8)
        // see missing bits and values in separate registers
        ENABLE = BIT29, //!< Enable the option of ULP coprocessor woken up by RTC GPIO
        RESET  = BIT30, //!< Disable the option of ULP coprocessor woken up by RTC GPIO. (WO) //TODO write only
    };
public:
    CoreFSM* getCoreFSM() { return fsm; }
    CoreRISCV* getCoreRISCV() { return riscv; }
    TimerULP* getTimerULP() { return timer; }
    bool getConfig( const ConfigGPIO test ) const { return cfgGPIO->get( static_cast<uint32_t>( test ) ); };
    void setConfig( const ConfigGPIO mask, bool value ) { cfgGPIO->set( static_cast<uint32_t>( mask ), value ); };
};

#endif
