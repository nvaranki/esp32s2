// FSM (finite state machine) core of ULP co-processor.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "CoreFSM.hpp"

CoreFSM::CoreFSM() :
    isz( new SubValueRW( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_MEM_ADDR_INIT_M, RTC_CNTL_ULP_CP_MEM_ADDR_INIT_S ) ), //TODO unspecified
    cmd( new BitSetWO( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_MEM_OFFST_CLR_M ) ), //TODO unspecified
    asz( new SubValueRW( RTC_CNTL_ULP_CP_CTRL_REG, RTC_CNTL_ULP_CP_MEM_ADDR_SIZE_M, RTC_CNTL_ULP_CP_MEM_ADDR_SIZE_S ) ), //TODO unspecified
    cfg( new BitSetRW( RTC_CNTL_ULP_CP_CTRL_REG, 0xF0000000 ) ) //TODO predefined constants
{
}

CoreFSM::~CoreFSM()
{
    delete isz;
    delete cmd;
    delete asz;
    delete cfg;
}

void CoreFSM::start()
{
    // 0->1 transition starts the processor
    cfg->set( static_cast<uint32_t>( Configuration::STARTS ), false );
    cfg->set( static_cast<uint32_t>( Configuration::STARTS ), true );
}
