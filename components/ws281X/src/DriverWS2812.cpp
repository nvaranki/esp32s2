/* 
 * Single/pair/chain WS2812 LED driver. 
 * Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#include "DriverWS2812.hpp"

DriverWS2812::DriverWS2812( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio ) :
    DriverWS281X( mcu, ch, gpio, T0H, T0L, T1H, T1L, RES )
{
}

DriverWS2812::~DriverWS2812()
{
}
