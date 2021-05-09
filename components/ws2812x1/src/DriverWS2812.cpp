/* 
 * Single/pair WS2812 LED driver. Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#include <stdio.h>
#include <initializer_list>
#include "DriverWS2812.hpp"

constexpr uint16_t T0H = 350u; // 0 code, high voltage time, ns
constexpr uint16_t T0L = 800u; // 0 code,  low voltage time, ns
constexpr uint16_t T1H = 700u; // 1 code, high voltage time, ns
constexpr uint16_t T1L = 600u; // 1 code,  low voltage time, ns
constexpr uint16_t RES = 50000u; // low voltage time, ns

DriverWS2812::DriverWS2812( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio ) :
    SingleNZR( 
        mcu->periphery.getRemoteControlController(), 
        mcu->periphery.getRemoteControlController()->getChannel( ch ),
        mcu->getControllerIO()->getExternalPin( gpio ), 
        mcu->getControllerIO()->getMatrixOutput( gpio ),
        // APB frequency 80 MHz -> 12.5 ns == 25/2 ns
        T0H * 2 / 25, T0L * 2 / 25, T1H * 2 / 25, T1L * 2 / 25, RES * 2 / 25 + 1 )
{
}

DriverWS2812::~DriverWS2812()
{
}

int DriverWS2812::send( const uint8_t r, const uint8_t g, const uint8_t b )
{
    const uint8_t values[] { g, r, b };
    return sendz( values, sizeof(values)/sizeof(uint8_t) );
}

int DriverWS2812::send( 
        const uint8_t r0, const uint8_t g0, const uint8_t b0, 
        const uint8_t r1, const uint8_t g1, const uint8_t b1 )
{
    const uint8_t values[] { g0, r0, b0, g1, r1, b1 };
    return sendz( values, sizeof(values)/sizeof(uint8_t) );
}
