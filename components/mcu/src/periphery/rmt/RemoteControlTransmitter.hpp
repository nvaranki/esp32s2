// Generator of signal and transmitter of it to the output.
// Includes carrier modulation. Acquires data from the RAM.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlTransmitter
#define H_RemoteControlTransmitter

#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"
#include "InterruptController.hpp"
#include "RemoteControlIdle.hpp"
#include "RemoteControlLimit.hpp"
#include "RemoteControlLoop.hpp"
#include "RemoteControlTransmitterCarrier.hpp"
#include "Trigger2.hpp"

class RemoteControlTransmitter
{
public:
    /** Carrier modulation of signal level. */
    RemoteControlTransmitterCarrier* const carrier;
    /** Configuration of continuous transmission mode */
    RemoteControlLoop* const loop;
    /** Configuration of idle state (no signal to send) */
    RemoteControlIdle* const idle;
    /** Configuration of transmission marker; used in continuous mode */
    RemoteControlLimit* const limit;
    /** Start/stop sending data out */
    Trigger2 const send;
    /** continuous mode (resets RAM read address automatically on transmission end-marker) */
    FlagRW* const continuous;
    /** start sending data simultaneously with other channels */
    FlagRW* const simultaneously;
    /** end of transmit interrupt status and control;
     * Triggered when the transmitter has finished transmitting signals.
     */
    InterruptController* const interrupt;
    /** Index of the GPIO Matrix output channel */
    uint8_t const index;
public:
    RemoteControlTransmitter( const size_t channel );
    virtual ~RemoteControlTransmitter();
};

#endif
