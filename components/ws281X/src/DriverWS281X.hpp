/* 
 * Single/pair/chain WS281* family LED driver. 
 * Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#ifndef H_DriverWS281X
#define H_DriverWS281X

#include <stdint.h>
#include "MicroControllerUnit.hpp"
#include "snzr.h"

class DriverWS281X : public SingleNZR
{
protected:
    /**
     * @param mcu  microcontroller unit driver
     * @param ch   RMT channel number
     * @param gpio GPIO pin number
     * @param T0H  0 code, high voltage time, ns
     * @param T0L  0 code,  low voltage time, ns
     * @param T1H  1 code, high voltage time, ns
     * @param T1L  1 code,  low voltage time, ns
     * @param TRST terminating low voltage time, ns
     */
    DriverWS281X( 
        MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio,
        const uint16_t T0H, const uint16_t T0L, const uint16_t T1H, const uint16_t T1L, 
        const uint16_t RES );
    virtual ~DriverWS281X();
public:
    /**
     * Transmits RGB setting to single LED device.
     * @param r red color intensity, [0,255]
     * @param g green color intensity, [0,255]
     * @param b blue color intensity, [0,255]
     * @return number of actually processed bytes; 3 in success
     */
    int send( const uint8_t r, const uint8_t g, const uint8_t b );
    /**
     * Transmits RGB setting to a chain of two LED devices.
     * @param r0 first LED red color intensity, [0,255]
     * @param g0 first LED green color intensity, [0,255]
     * @param b0 first LED blue color intensity, [0,255]
     * @param r1 next LED red color intensity, [0,255]
     * @param g1 next LED green color intensity, [0,255]
     * @param b1 next LED blue color intensity, [0,255]
     * @return number of actually processed bytes; 6 in success
     */
    int send( const uint8_t r0, const uint8_t g0, const uint8_t b0, 
               const uint8_t r1, const uint8_t g1, const uint8_t b1 );
    /**
     * Transmits GRB color triples to a chain of LED devices.
     * @param data each triple of bytes in sequence serves GRB color intensity, [0,255] each, for LEDs in a chain;  
     *     triple at indices 0-2 goes out for first LED, then at indices 3-5 for second LED, an so on;
     * @param size number of bytes in array, must be a multiple of 3
     * @return number of actually processed bytes
     */
    int send( const uint8_t* const data, uint32_t const size );
};

#endif
