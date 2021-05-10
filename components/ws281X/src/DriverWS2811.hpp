/* 
 * Single/pair/chain WS2811 LED driver. 
 * Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#ifndef H_DriverWS2811
#define H_DriverWS2811

#include <stdint.h>
#include "DriverWS281X.hpp"
#include "MicroControllerUnit.hpp"

class DriverWS2811 final : public DriverWS281X
{
private:
    // It is one half of the time when high speed mode (reset time unchanged)
    static constexpr uint32_t T0H =   500u; //!< 0 code, high voltage time, ns
    static constexpr uint32_t T0L =  2000u; //!< 0 code,  low voltage time, ns
    static constexpr uint32_t T1H =  1200u; //!< 1 code, high voltage time, ns
    static constexpr uint32_t T1L =  1300u; //!< 1 code,  low voltage time, ns
    static constexpr uint32_t RES = 50000u; //!< low voltage time, ns
public:
    /**
     * @param mcu  microcontroller unit driver
     * @param ch   RMT channel number
     * @param gpio GPIO pin number
     * @param hs   high speed mode
     */
    DriverWS2811( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio, const bool hs );
    virtual ~DriverWS2811();
};

#endif
