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
#include "Trigger2.hpp"

class CoreFSM
{
private:
    FlagRW* const exec; //!< 1(default): select ULP-FSM
    FlagRW* const done; //!< 0: select ULP-FSM DONE signal
public:
    /**
     * When "on" turned to true and "off" turned to false, it provides 
     * clock ticks for the core. Other combinations effectively gate 
     * clock ticks from the core.
     */
    Trigger2 const clock;
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
public:
    /** Selects this core to execute program. */
    void selectForExec() { exec->set( true ); }
    /** Selects this core to send DONE signal to ULP timer after the program has been finished. */
    void selectForDone() { done->set( false ); }
};

#endif
