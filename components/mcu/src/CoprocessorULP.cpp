// Ultra low power (ULP) co-processor.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "esp32s2/ulp.h"
#include "CoprocessorULP.hpp"

CoprocessorULP::CoprocessorULP() :
    fsm( new CoreFSM() ),
    riscv( new CoreRISCV() ),
    timer( new TimerULP() ),
    cfgCore( new RegisterRW( RTC_CNTL_COCPU_CTRL_REG, 0x01800000 ) ), //TODO predefined constants
    cfgGPIO( new RegisterRW( RTC_CNTL_ULP_CP_TIMER_REG, 0x60000000 ) ), //TODO predefined constants
    addr(  new ValueRW( RTC_CNTL_ULP_CP_TIMER_REG, 0x7FF, 0 ) ) //TODO predefined constants
{
}
    
CoprocessorULP::~CoprocessorULP()
{
    delete fsm;
    delete riscv;
    delete timer;
    delete cfgCore;
    delete cfgGPIO;
    delete addr;
}

esp_err_t CoprocessorULP::loadExecCode( const uint32_t addr, const uint8_t code[] , const size_t size )
{
    return ulp_load_binary( addr, code, size ); //TODO move code here from ulp.c
}
