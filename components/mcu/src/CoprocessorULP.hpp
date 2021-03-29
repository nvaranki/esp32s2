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
