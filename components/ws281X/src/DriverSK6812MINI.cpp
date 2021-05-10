/* 
 * Single/pair/chain SK6812MINI LED driver. 
 * Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#include "DriverSK6812MINI.hpp"

DriverSK6812MINI::DriverSK6812MINI( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio ) :
    DriverWS281X( mcu, ch, gpio, T0H, T0L, T1H, T1L, RES )
{
}

DriverSK6812MINI::~DriverSK6812MINI()
{
}
