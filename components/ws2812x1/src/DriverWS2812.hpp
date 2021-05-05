/* 
 * Single/pair WS2812 LED driver. Employs RMT output channel.
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
     * Transmits RGB setting to single LED device.
     * @param r red color intensity, [0,255]
     * @param g green color intensity, [0,255]
     * @param b blue color intensity, [0,255]
     */
    void send( const uint8_t r, const uint8_t g, const uint8_t b );
    /**
     * Transmits RGB setting to a chain of two LED devices.
     * @param r0 first LED red color intensity, [0,255]
     * @param g0 first LED green color intensity, [0,255]
     * @param b0 first LED blue color intensity, [0,255]
     * @param r1 next LED red color intensity, [0,255]
     * @param g1 next LED green color intensity, [0,255]
     * @param b1 next LED blue color intensity, [0,255]
     */
    void send( const uint8_t r0, const uint8_t g0, const uint8_t b0, 
               const uint8_t r1, const uint8_t g1, const uint8_t b1 );
private:
    void load( uint8_t* const values, uint32_t const size );
    void push();
};

#endif
