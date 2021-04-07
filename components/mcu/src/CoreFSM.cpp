// FSM (finite state machine) core of ULP co-processor.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "CoreFSM.hpp"

CoreFSM::CoreFSM() :
    exec( new FlagRW( RTC_CNTL_COCPU_CTRL_REG, RTC_CNTL_COCPU_SEL_S ) ),
    done( new FlagRW( RTC_CNTL_COCPU_CTRL_REG, RTC_CNTL_COCPU_DONE_FORCE_S ) ),
    clock( 
        new FlagRW( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_CLK_FO_S ),
        new FlagRW( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_RESET_S ) ),
    startOn( new FlagRW( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_START_TOP_S ) ),
    start( new FlagRW( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_FORCE_START_TOP_S ) ),
    memoryAddressSize( new SubValueRW( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_MEM_ADDR_SIZE_M, RTC_CNTL_ULP_CP_MEM_ADDR_SIZE_S ) ),
    memoryAddressInit( new SubValueRW( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_MEM_ADDR_INIT_M, RTC_CNTL_ULP_CP_MEM_ADDR_INIT_S ) ),
    memoryOffsetClear( new FlagWO( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_MEM_OFFST_CLR_S ) )
{
}

CoreFSM::~CoreFSM()
{
    delete exec;
    delete done;
    delete clock.on;
    delete clock.off;
    delete startOn;
    delete start;
    delete memoryAddressSize;
    delete memoryAddressInit;
    delete memoryOffsetClear;
}
