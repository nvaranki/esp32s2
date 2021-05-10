/* 
 * Single/pair/chain WS281* family LED driver. 
 * Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#include <stdio.h>
#include "DriverWS281X.hpp"

DriverWS281X::DriverWS281X( 
        MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio,
        const uint32_t T0H, const uint32_t T0L, const uint32_t T1H, const uint32_t T1L, 
        const uint32_t RES ) :
    SingleNZR( 
        mcu->periphery.getRemoteControlController(), 
        mcu->periphery.getRemoteControlController()->getChannel( ch ),
        mcu->getControllerIO()->getExternalPin( gpio ), 
        mcu->getControllerIO()->getMatrixOutput( gpio ),
        // APB frequency 80 MHz -> 12.5 ns == 25/2 ns
        T0H * 2 / 25, T0L * 2 / 25, T1H * 2 / 25, T1L * 2 / 25, RES * 2 / 25 )
{
}

DriverWS281X::~DriverWS281X()
{
}

int DriverWS281X::send( const uint8_t r, const uint8_t g, const uint8_t b )
{
    Color values;
    values.r = r;
    values.g = g;
    values.b = b;
    int rc = transmit( &values.g, 3, SingleNZR::BitOrder::MSBF );
    // printf( "sent=0x%-8x\n", rc );
    return rc;
}

int DriverWS281X::send( 
        const uint8_t r0, const uint8_t g0, const uint8_t b0, 
        const uint8_t r1, const uint8_t g1, const uint8_t b1 )
{
    Color values[2];
    values[0].r = r0;
    values[0].g = g0;
    values[0].b = b0;
    values[1].r = r1;
    values[1].g = g1;
    values[1].b = b1;
    return transmit( &values[0].g, 6, SingleNZR::BitOrder::MSBF );
}

int DriverWS281X::send( const uint8_t* const data, uint32_t const size )
{
    return transmit( data, size, SingleNZR::BitOrder::MSBF );
}

int DriverWS281X::send( const Color* const data, uint32_t const size )
{
    int rc = transmit( &data->g, size * 3, SingleNZR::BitOrder::MSBF );
    // printf( "sent=0x%-8x\n", rc );
    return rc;
}
