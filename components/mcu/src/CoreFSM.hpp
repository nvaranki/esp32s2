// FSM (finite state machine) core of ULP co-processor.
//
// © 2021 Nikolai Varankine

#ifndef H_CoreFSM
#define H_CoreFSM

#include "esp_bit_defs.h"
#include "soc/rtc_cntl_reg.h"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRW.hpp"

class CoreFSM
{
public:
    /**
     * When turned to true, it provides clock ticks for the core. Property
     * "clockOff" should be false to allow ticks to reach the core.
     */
    FlagRW* const clockOn;
    /**
     * When turned to true, it blocks clock ticks for the core. Property
     * "clockOn" should be true to allow ticks to reach the core.
     */
    FlagRW* const clockOff;
    /**
     * When turned to true, it allows the core to start execution of a program. This 
     * property controls ability of the core to start only by manupulation with 
     * "start" property. It has no effect on how and when a timer starts the core.
     */
    FlagRW* const startOn;
    /**
     * When turned from false to true, it immediately forces the core to run 
     * program from entry point, provided that such start is allowed by "startOn"
     * property.
     */
    FlagRW* const start;
    // TODO undocumented:
    SubValueRW* const memoryAddressSize; //!< memory address size
    SubValueRW* const memoryAddressInit; //!< init memory address size?
    FlagWO*     const memoryOffsetClear; //!< unknown?
public:
    CoreFSM();
    virtual ~CoreFSM();
};

#endif
