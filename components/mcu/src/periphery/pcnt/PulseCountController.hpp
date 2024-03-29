// Periphery Unit: multi-channel pulse counter.
//
// © 2021 Nikolai Varankine

#ifndef H_PulseCountController
#define H_PulseCountController

#include <cstddef>
#include "soc/pcnt_caps.h"
#include "bits/FlagRW.hpp"
#include "bits/WordRO.hpp"
#include "PulseCountUnit.hpp"

class PulseCountController
{
public:
    static constexpr size_t MAX_UNIT = SOC_PCNT_UNIT_NUM;
public:
    /** Module clock control */
    FlagRW* const clock;
    /** Module reset control */
    FlagRW* const reset;
    /** The registers clock gate enable signal of PCNT module. 
     * 1: the registers can be read and written by application. 
     * 0: the registers can not be read or written by application 
     */
    FlagRW* const enable;
    /** Hardware version */
    WordRO* const version;
private:
    PulseCountUnit* ctrl[MAX_UNIT];
public:
    PulseCountController();
    virtual ~PulseCountController();
public:
    PulseCountUnit* getUnit( const size_t i );
};

#endif
