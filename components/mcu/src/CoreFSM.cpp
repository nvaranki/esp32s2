// FSM (finite state machine) core of ULP co-processor.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "CoreFSM.hpp"

CoreFSM::CoreFSM() :
    cfg( new BitSetRW( RTC_CNTL_ULP_CP_CTRL_REG, 0xF0000000 ) ) //TODO predefined constants
{
}

CoreFSM::~CoreFSM()
{
    delete cfg;
}
