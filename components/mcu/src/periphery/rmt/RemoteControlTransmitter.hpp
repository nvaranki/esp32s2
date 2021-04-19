// Generator of signal and transmitter of it to the output.
// Includes carrier modulation. Acquires data from the RAM.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlTransmitter
#define H_RemoteControlTransmitter

#include "bits/FlagRW.hpp"
#include "bits/FlagRO.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "Trigger2.hpp"

class RemoteControlTransmitter
{
public:
    /** Start/stop sending data out */
    Trigger2 const send;
    /** Total number of signal entries (level and duration) to send out, max 128=2*64 per allocated RAM block */
    SubValueRW* const entries;
    /** reset RAM read address */
    FlagRW* const repeat;
    /** start sending data simultaneously with other channels */
    FlagRW* const simultaneously;
    /** RAM address to write received signal */
    class Address
    {
    public:
        /** memory address offset when channel reads RAM via APB bus */
        SubValueRW* const offset;
        /** current memory address offset when channel reads RAM via APB bus */
        SubValueRO* const current;
        /** reset RAM read address */
        FlagWO* const reset;
        /** offset address is out of memory size */
        FlagRO* const error;
        /** more data to be sent than the memory allows, and the wrap mode is disabled */
        FlagRO* const full;
    public:
        Address( const size_t channel );
        virtual ~Address();
    }
    const address;
    /** Carrier modulation of signal level. */
    class Carrier
    {
    public:
        /** perform carrier modulation */
        FlagRW* const modulation;
        /** Limit states when carrier modulation is performed.
         * 1: modulate only at data sending state; 
         * 0: modulate at all states (idle, reading data from RAM, and sending data stored in RAM) */
        FlagRW* const limited;
        /** signal level to apply carrier modulation */
        FlagRW* const level;
        /** Number (low +1) of clock cycles in period of carrier wave at low signal TODO ?*/
        SubValueRW* const low;
        /** Number (high +1) of clock cycles in period of carrier wave at high signal TODO ?*/
        SubValueRW* const high;
    public:
        Carrier( const size_t channel );
        virtual ~Carrier();
    }
    const carrier;
    /** Configuration of continuous transmission mode */
    class Loop
    {
    public:
        /** enable loop counting */
        FlagRW* const enable;
        /** maximum loop times */
        SubValueRW* const repeat;
        /** reset loop counting when continuous transmission mode is valid */
        FlagRW* const reset;
    public:
        Loop( const size_t channel );
        virtual ~Loop();
    }
    const loop;
    /** Configuration of idle state (no signal to send) */
    class Idle
    {
    public:
        /** output enable for the channel when in idle state */
        FlagRW* const output;
        /** level of output signal when in idle state */
        FlagRW* const level;
    public:
        Idle( const size_t channel );
        virtual ~Idle();
    }
    const idle;
public:
    RemoteControlTransmitter( const size_t channel );
    virtual ~RemoteControlTransmitter();
};

#endif
