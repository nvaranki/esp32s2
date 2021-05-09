/* 
 * Single/pair/chain WS2811 LED driver. 
 * Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#include "DriverWS2811.hpp"

DriverWS2811::DriverWS2811( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio, const bool hs ) :
    DriverWS281X( mcu, ch, gpio, T0H / ( hs + 1u ), T0L / ( hs + 1u ), T1H / ( hs + 1u ), T1L / ( hs + 1u ), RES )
{
}

DriverWS2811::~DriverWS2811()
{
}
