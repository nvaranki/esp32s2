// Main Xtensa® LX7 processor core.
//
// © 2021 Nikolai Varankine

#ifndef H_CoreLX7
#define H_CoreLX7

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"

class CoreLX7
{
private:
    FlagRW* const staticVector; //!< Selects the CPU Core 0 (PRO) static boot vector
//  FlagRW* const staticVector; //!< Selects the CPU Core 1 (APP) static boot vector TODO unsupported
public:
    /**
     * When set to true, immediately resets this particular CPU core only. 
     * Other domains stay unaffected. Once such reset is released, CPU core will 
     * start execution of program code from address stored in CPU reset vector.
     */
    FlagWO* const reset;
private:
    SubValueRO* const resetCause; //!< reset cause of the CPU Core 0 (PRO)
//  SubValueRO* const resetCause; //!< reset cause of the CPU Core 1 (APP) TODO unsupported
    SubValueRW* const control; //!< setting this bit to 0x21 stalls the CPU Core 0 (PRO), see MicroControllerUnit::stall
//  SubValueRW* const control; //!< setting this bit to 0x21 stalls the CPU Core 1 (APP), see MicroControllerUnit::stall TODO unsupported
public:
    const int id;
public:
    CoreLX7( const int id );
    virtual ~CoreLX7();
public:
    /**
     * A reason why system has been reset. See Table 7: Reset Source.
     */
    enum class ResetCause : uint32_t
    {
        UNSET  = 0x00,
        // Chip Reset
        CHIP   = 0x01, //!< Chip reset (power on, reset button) - Chip Reset - See the note in the table 7
        // System Reset
        BROWN  = 0x0F, //!< Brown-out system reset - System Reset - Triggered by brown-out detector
        RWDTS  = 0x10, //!< RWDT system reset - System Reset - See Chapter 15 Watchdog Timers (WDT)
        GLITCH = 0x13, //!< GLITCH reset - System Reset -
        // Core Reset
        SYSTEM = 0x03, //!< Software system reset - Core Reset - Triggered by configuring RTC_CNTL_SW_SYS_RST
        SLEEP  = 0x05, //!< Deep-sleep reset - Core Reset - See Chapter 32 Low-Power Management (RTC_CNTL)
        MWDT0G = 0x07, //!< MWDT0 global reset - Core Reset - See Chapter 15 Watchdog Timers (WDT)
        MWDT1G = 0x08, //!< MWDT1 global reset - Core Reset - See Chapter 15 Watchdog Timers (WDT)
        RWDTC  = 0x09, //!< RWDT core reset - Core Reset - See Chapter 15 Watchdog Timers (WDT)
        // CPU Reset
        MWDT0  = 0x0B, //!< MWDT0 CPU reset - CPU Reset - See Chapter 15 Watchdog Timers (WDT)
        SOFT   = 0x0C, //!< Software CPU reset - CPU Reset - Triggered by configuring RTC_CNTL_SW_PROCPU_RST
        RWDTP  = 0x0D, //!< RWDT CPU reset - CPU Reset - See Chapter 15 Watchdog Timers (WDT)
        MWDT1  = 0x11, //!< MWDT1 CPU reset - CPU Reset - See Chapter 15 Watchdog Timers (WDT)
    };
    enum class Command : uint32_t
    {
        CLEAR = 0x00, //!< clears command register
        STALL = 0x21, //!< stalls the CPU Core
    };
    enum class BootVectorRTC : bool
    {
        SLOW = 0, //!< Using RTC slow memory from 0x50000000
        FAST = 1, //!< Using ROM from 0x40000400, then jump to RTC fast memory at RTC_CNTL_STORE6_REG
    };
public:
    ResetCause getResetCause();
    void command( Command c );
    void setBootVectorRTC( BootVectorRTC v );
    BootVectorRTC getBootVectorRTC() const;
};

#endif
