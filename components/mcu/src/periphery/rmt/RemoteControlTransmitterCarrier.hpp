// Carrier modulator management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlTransmitterCarrier
#define H_RemoteControlTransmitterCarrier

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class RemoteControlTransmitterCarrier
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
    RemoteControlTransmitterCarrier( const uint32_t channel );
    virtual ~RemoteControlTransmitterCarrier();
};

#endif
