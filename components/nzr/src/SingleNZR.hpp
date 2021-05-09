/* 
 * Single wire asynchronous data transfer in NZR communication mode; employs RMT output channel.
 * 
 * Every data bit in sequence is transferred by one of two predefined pulses of specific duty cycles, 
 * whether it is 0 or 1. Entire bit set is finished by pulse of third type, also of specific duty cycle.
 * Signal patterns are measured in channel's divided clock ticks.
 * 
 * Author © 2021 Nikolai Varankine
 */

#ifndef H_SingleNZR
#define H_SingleNZR

#include <stdint.h>
#include "soc/rmt_struct.h"
#include "io/ExternalPin.hpp"
#include "io/MatrixOutput.hpp"
#include "periphery/rmt/RemoteControlController.hpp"
#include "periphery/rmt/RemoteControlChannel.hpp"

class SingleNZR 
{
public:
    /** RMT controller */
    const RemoteControlController* const rmt;
    /** RMT channel */
    RemoteControlChannel* const channel;
    /** GPIO pin to get generated signal from */
    ExternalPin* const pin;
    /** RMT channel output in GPIO Matrix */
    MatrixOutput* const output;
private:
    /** 0 and 1 signal patterns, i.e. elements of "data refresh cycle" */
    const rmt_item32_t DATA[2];
    /** signal pattern of terminatot, i.e. a "reset code" */
    const rmt_item32_t RESET;
    /** safety threshold for interrupt wait cycles */
    const uint32_t thrLoop;
public:
    /**
     * @param rmt     RMT controller
     * @param channel RMT channel
     * @param pin     GPIO pin to get generated signal from
     * @param output  RMT channel output in GPIO Matrix
     * @param n0H     0 code, high voltage time, pulses
     * @param n0L     0 code,  low voltage time, pulses
     * @param n1H     1 code, high voltage time, pulses
     * @param n1L     1 code,  low voltage time, pulses
     * @param nRST    terminating low voltage time, pulses
     */
    SingleNZR( const RemoteControlController* const rmt, RemoteControlChannel* const channel, 
        ExternalPin* const pin, MatrixOutput* const output,
        const uint16_t n0H, const uint16_t n0L, 
        const uint16_t n1H, const uint16_t n1L, 
        const uint16_t nRST );
    virtual ~SingleNZR();
public:
    enum class BitOrder : uint8_t
    {
        /** 
         * most significant bit goes out first;
         * i.e. 0x80, 0x40,... 0x01
         */
        MSBF = 0x80u,
        /** 
         * least significant bit goes out first;
         * i.e. 0x01, 0x02,... 0x80
         */
        LSBF = 0x01u,
    };
    /**
     * Transmits bit sequence to destination. Every bit is converted 
     * into two RMT entries comprizing on-off pair in output signal. 
     * @param data bit sequence as byte array
     * @param size of array
     * @param order bit priority
     * @return number of actually processed bytes
     */
    int transmit( const uint8_t* const data, uint32_t const size, const BitOrder order );
private:
    uint32_t loadBytes( const uint8_t* const values, uint32_t const size, const BitOrder order,
        uint32_t const start, uint32_t const bound, const bool direct );
    void loadEntries( const rmt_item32_t& data, 
        uint32_t const start, uint32_t const bound, const bool direct );
    bool waitFor( InterruptController* const interrupt );
};

#endif
