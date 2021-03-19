// RISC-V core of ULP co-processor.
//
// © 2021 Nikolai Varankine

#ifndef H_CoreRISCV
#define H_CoreRISCV

#include "esp_bit_defs.h"
#include "bits/BitSetRW.hpp"
#include "bits/SubValueRW.hpp"

class CoreRISCV
{
private:
    BitSetRW* const cfg; //!< configuration register
    SubValueRW* const s2r; //!< Time from startup to pull down reset.
    SubValueRW* const s2i; //!< Time from startup to send out RISCV_START_INT interrupt.
    SubValueRW* const s2dc; //!< Time from shut down to disable clock.
public:
    CoreRISCV();
    virtual ~CoreRISCV();
public:
    enum class Configuration : uint32_t
    {
        // RTC_CNTL_COCPU_CTRL_REG (0x0100)
        // see missing bits and values in separate registers
        CLKFO = BIT0, //!< clock force on
        SHUT  = BIT13, //!< shut down the processor
        RESET = BIT22, //!< reset the processor
        DONE  = BIT25, //!< 1: send DONE signal and processor will go to HALT and the timer starts counting
        INT   = BIT26, //!< 1: trigger register interrupt; WO //TODO write-only bit
    };
public:
    bool getConfig( const Configuration test ) const { return cfg->get( static_cast<uint32_t>( test ) ); };
    void setConfig( const Configuration mask, bool value ) { cfg->set( static_cast<uint32_t>( mask ), value ); };
};

#endif