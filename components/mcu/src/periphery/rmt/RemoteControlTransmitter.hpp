// Generator of signal and transmitter of it to the output.
// Includes carrier modulation. Acquires data from the RAM.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlTransmitter
#define H_RemoteControlTransmitter

#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"
#include "RemoteControlTransmitterAddress.hpp"
#include "RemoteControlTransmitterCarrier.hpp"
#include "RemoteControlLoop.hpp"
#include "RemoteControlIdle.hpp"
#include "Trigger2.hpp"

class RemoteControlTransmitter
{
public:
    /** RAM address to write received signal */
    RemoteControlTransmitterAddress* const address;
    /** Carrier modulation of signal level. */
    RemoteControlTransmitterCarrier* const carrier;
    /** Configuration of continuous transmission mode */
    RemoteControlLoop* const loop;
    /** Configuration of idle state (no signal to send) */
    RemoteControlIdle* const idle;
    /** Start/stop sending data out */
    Trigger2 const send;
    /** Total number of signal entries (level and duration) to send out, max 128=2*64 per allocated RAM block */
    SubValueRW* const entries;
    /** reset RAM read address */
    FlagRW* const repeat;
    /** start sending data simultaneously with other channels */
    FlagRW* const simultaneously;
public:
    RemoteControlTransmitter( const size_t channel );
    virtual ~RemoteControlTransmitter();
};

#endif
