// Controller of GPIO pin.
//
// © 2021 Nikolai Varankine

#ifndef H_ExternalPin
#define H_ExternalPin

#include <stdint.h>
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "bits/BitSetRW.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class ExternalPin
{
public:
    /** Sleep mode of the pin */
    class Sleep
    {
    public:
        enum class Pull : uint32_t
        {
            /** leave the pin open (up/down state is defined by external load) */
            OPEN = 0,
            /** pull-down the pin to GND with internal resistor */
            DOWN = SLP_PD_M,
            /** pull-up the pin to VCC with internal resistor */
            UP = SLP_PU_M,
        };
    private:
        /** Select pull-up/down or open mode */
        BitSetRW* const pull;
    public:
        /** Set to 1 to put the pad in sleep mode */
        FlagRW* const enable;
        /** Enable input signals */
        FlagRW* const input;
        /** Enable output signals */
        FlagRW* const output;
    public:
        Sleep( const size_t i );
        virtual ~Sleep();
    public:
        Pull getPull() const;
        void setPull( Pull v );
    }
    const sleep;
    enum class DriveStrength : uint32_t
    {
        /** drive strength of the pad ~5 mA */
        OUT5MA  = 0,
        /** drive strength of the pad ~10 mA */
        OUT10MA = 1,
        /** drive strength of the pad ~20 mA */
        OUT20MA = 2,
        /** drive strength of the pad ~40 mA */
        OUT40MA = 3,
    };
    enum class Pull : uint32_t
    {
        /** leave the pin open (up/down state is defined by external load) */
        OPEN = 0,
        /** pull-down the pin to GND with internal resistor */
        DOWN = FUN_PD_M,
        /** pull-up the pin to VCC with internal resistor */
        UP = FUN_PU_M,
    };
    /** Stage of GPIO Matrix input synchronization cascade */
    enum class SyncStage : uint8_t
    {
        FIRST  = 1,
        SECOND = 2,
    };
    /** GPIO Matrix input synchronization with APB clock */
    enum class SyncMode : uint32_t
    {
        /** no synchronization */
        BYPASS = 0,
        /** synchronized on falling edge */
        FALL = 1,
        /** synchronized on rising edge */
        RAISE = 2,
    };
private:
    /** Output drive strength */
    SubValueRW* const strength;
    /** Select pull-up/down or open mode */
    BitSetRW* const pull;
    /** First stage synchronization */
    SubValueRW* const sync1;
    /** Second stage synchronization */
    SubValueRW* const sync2;
public:
    /** Enable input signals */
    FlagRW* const input;
    /** IO MUX function defined as FUNC_<pinname_function>, see the "Table 23: IO MUX Pad List" */
    SubValueRW* const function;
    /** Enable filter for matrix input signals */
    FlagRW* const filter;
    /** GPIO pin number */
    size_t const number;
public:
    /** @param i GPIO pin number */
    ExternalPin( const size_t i );
    virtual ~ExternalPin();
public:
    DriveStrength getDriveStrength() const;
    void setDriveStrength( DriveStrength v );
    Pull getPull() const;
    void setPull( Pull v );
    SyncMode getSync( const SyncStage stage ) const;
    void setSync( const SyncStage stage, const SyncMode mode );
};

#endif
