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
    BitSetRW* const cfgCore; //!< configuration register
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
    enum class ConfigCore : uint32_t
    {
        // RTC_CNTL_COCPU_CTRL_REG (0x0100)
        // see missing bits and values in separate registers
        CORE  = RTC_CNTL_COCPU_SEL, //!< 0: select ULP-RISC-V; 1(default): select ULP-FSM
        DONE  = RTC_CNTL_COCPU_DONE_FORCE, //!< 0: select ULP-FSM DONE signal; 1: select ULP-RISC-V DONE signal
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
    bool getConfig( const ConfigCore test ) const { return cfgCore->get( static_cast<uint32_t>( test ) ); };
    void setConfig( const ConfigCore mask, bool value ) { cfgCore->set( static_cast<uint32_t>( mask ), value ); };
    bool getConfig( const ConfigGPIO test ) const { return cfgGPIO->get( static_cast<uint32_t>( test ) ); };
    void setConfig( const ConfigGPIO mask, bool value ) { cfgGPIO->set( static_cast<uint32_t>( mask ), value ); };
    /**
     * @param address memory location to load binary at, usually beginning of RTC slow memory (RTC_SLOW_MEM) with optional offset.
     * @param image compiled binary image.
     * @param size size of the image, in bytes.
     * @return completion code.
     */
    esp_err_t loadExecCode( uint32_t* address, const uint8_t* image , const size_t size );
    /**
     * @param addr offset (in 32b words) from the beginning of RTC slow memory (RTC_SLOW_MEM).
     */
};

#define ULP_BINARY_MAGIC_ESP32 (0x00706c75)

#endif
