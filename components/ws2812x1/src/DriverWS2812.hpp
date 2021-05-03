/* 
 * Single WS2812 LED driver. Employs RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#ifndef H_DriverWS2812
#define H_DriverWS2812

#include "stdint.h"
#include "soc/rmt_struct.h"
#include "io/ExternalPin.hpp"
#include "io/MatrixOutput.hpp"
#include "MicroControllerUnit.hpp"

class DriverWS2812 final
{
private:
    ControllerIO* const io;
    RemoteControlController* const rmt;
    RemoteControlChannel* const rcc;
    ExternalPin* const pin;
    MatrixOutput* const output;
    const rmt_item32_t DATA[2];
    const rmt_item32_t RESET;
public:
    /**
     * @param mcu  microcontroller unit driver
     * @param ch   RMT channel number
     * @param gpio GPIO pin number
     */
    DriverWS2812( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio );
    virtual ~DriverWS2812();
public:
    ControllerIO* getControllerIO() const { return io; };
    RemoteControlController* getController() const { return rmt; };
    RemoteControlChannel* getChannel() const { return rcc; };
    ExternalPin* getPin() const { return pin; };
    MatrixOutput* getOutput() const { return output; };
    /**
     * Transmits RGB setting to the LED device.
     * @param r red color intensity, [0,255]
     * @param g green color intensity, [0,255]
     * @param b blue color intensity, [0,255]
     */
    void send( const uint8_t r, const uint8_t g, const uint8_t b );
};

#endif
