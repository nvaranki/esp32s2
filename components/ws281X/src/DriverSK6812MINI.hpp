/* 
 * Single/pair/chain SK6812MINI LED driver. 
 * Employs NZR communication mode through RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#ifndef H_DriverSK6812MINI
#define H_DriverSK6812MINI

#include <stdint.h>
#include "DriverWS281X.hpp"
#include "MicroControllerUnit.hpp"

class DriverSK6812MINI final : public DriverWS281X
{
private:
    static constexpr uint32_t T0H =   300u; //!< 0 code, high voltage time, ns
    static constexpr uint32_t T0L =   900u; //!< 0 code,  low voltage time, ns
    static constexpr uint32_t T1H =   600u; //!< 1 code, high voltage time, ns
    static constexpr uint32_t T1L =   600u; //!< 1 code,  low voltage time, ns
    static constexpr uint32_t RES = 80000u; //!< low voltage time, ns
public:
    /**
     * @param mcu  microcontroller unit driver
     * @param ch   RMT channel number
     * @param gpio GPIO pin number
     */
    DriverSK6812MINI( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio );
    virtual ~DriverSK6812MINI();
};

#endif
