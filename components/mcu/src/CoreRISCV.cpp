// RISC-V core of ULP co-processor.
//
// © 2021 Nikolai Varankine

#include "soc/rtc_cntl_reg.h"
#include "CoreRISCV.hpp"

CoreRISCV::CoreRISCV() :
    cfg( new BitSetRW( RTC_CNTL_COCPU_CTRL_REG, 0x06402001 ) ), //TODO predefined constants
    s2r(  new SubValueRW( RTC_CNTL_COCPU_CTRL_REG, 0x3F<<1, 1 ) ), //TODO predefined constants
    s2i(  new SubValueRW( RTC_CNTL_COCPU_CTRL_REG, 0x3F<<7, 7 ) ), //TODO predefined constants
    s2dc( new SubValueRW( RTC_CNTL_COCPU_CTRL_REG, 0xFF<<14, 14 ) ) //TODO predefined constants
{
}

CoreRISCV::~CoreRISCV()
{
    delete cfg;
    delete s2r;
    delete s2i;
    delete s2dc;
}
