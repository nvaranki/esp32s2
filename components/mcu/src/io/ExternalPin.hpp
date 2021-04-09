// Controller of GPIO pin.
//
// © 2021 Nikolai Varankine

#ifndef H_ExternalPin
#define H_ExternalPin

#include <stdint.h>
#include "soc/io_mux_reg.h"
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
private:
    /** Output drive strength */
    SubValueRW* const strength;
    /** Select pull-up/down or open mode */
    BitSetRW* const pull;
public:
    /** Enable input signals */
    FlagRW* const input;
    /** IO MUX function, see FUNC_<pinname_function> */
    SubValueRW* const function;
    /** Enable filter for pin input signals */
    FlagRW* const filter;
public:
    ExternalPin( const size_t i );
    virtual ~ExternalPin();
public:
    DriveStrength getDriveStrength() const { return (DriveStrength) strength->get(); }
    void setDriveStrength( DriveStrength v ) { strength->set( static_cast<uint32_t>( v ) ); }
    Pull getPull() const;
    void setPull( Pull v );
};

#endif
