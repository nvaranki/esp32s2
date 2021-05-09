/* 
 * Single/pair/chain WS2812 LED driver. 
 * Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#ifndef H_DriverWS2812
#define H_DriverWS2812

#include <stdint.h>
#include "DriverWS281X.hpp"
#include "MicroControllerUnit.hpp"

class DriverWS2812 final : public DriverWS281X
{
private:
    static constexpr uint16_t T0H =   350u; //!< 0 code, high voltage time, ns
    static constexpr uint16_t T0L =   800u; //!< 0 code,  low voltage time, ns
    static constexpr uint16_t T1H =   700u; //!< 1 code, high voltage time, ns
    static constexpr uint16_t T1L =   600u; //!< 1 code,  low voltage time, ns
    static constexpr uint16_t RES = 50000u; //!< low voltage time, ns
public:
    /**
     * @param mcu  microcontroller unit driver
     * @param ch   RMT channel number
     * @param gpio GPIO pin number
     */
    DriverWS2812( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio );
    virtual ~DriverWS2812();
};

#endif
