// Ultra low power (ULP) co-processor.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "CoprocessorULP.hpp"

CoprocessorULP::CoprocessorULP() :
    fsm( new CoreFSM() ),
    riscv( new CoreRISCV() ),
    timer( new TimerULP() ),
//  cfgCore( new BitSetRW( RTC_CNTL_COCPU_CTRL_REG, 0x01800000 ) ), //TODO predefined constants
    cfgGPIO( new BitSetRW( RTC_CNTL_ULP_CP_TIMER_REG, 0x60000000 ) ), //TODO predefined constants
    entry( new SubValueRW( RTC_CNTL_ULP_CP_TIMER_REG, RTC_CNTL_ULP_CP_PC_INIT_M, RTC_CNTL_ULP_CP_PC_INIT_S ) )
{
}
    
CoprocessorULP::~CoprocessorULP()
{
    delete fsm;
    delete riscv;
    delete timer;
//  delete cfgCore;
    delete cfgGPIO;
    delete entry;
}

CoprocessorULP::Status::Status() :
    sleep( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_COCPU_STATE_SLP_S    ) ),
    work ( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_COCPU_STATE_SWITCH_S ) ),
    runs ( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_COCPU_STATE_START_S  ) ),
    done ( new FlagRO( RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_COCPU_STATE_DONE_S   ) )
{
}
    
CoprocessorULP::Status::~Status()
{
    delete sleep;
    delete work;
    delete runs;
    delete done;
}
