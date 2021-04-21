// Receiver of signal/modulation and decoder of it to the signal.
// Includes carrier remover. Trasmits data to the RAM.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlReceiver
#define H_RemoteControlReceiver

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"
#include "InterruptController.hpp"
#include "RemoteControlReceiverAddress.hpp"
#include "RemoteControlReceiverCarrier.hpp"
#include "RemoteControlFilter.hpp"

class RemoteControlReceiver
{
public:
    /** RAM address to write received signal */
    RemoteControlReceiverAddress* const address;
    /** Input sinal filter */
    RemoteControlFilter* const filter;
    /** Carrier remover configuration */
    RemoteControlReceiverCarrier* const carrier;
    /** allow to receive data */
    FlagRW* const enable;
    /** number 0f continuous clock cycles to detect idle state (no change on input) */
    SubValueRW* const idle;
    /** end of receive interrupt status and control;
     * Triggered when the receiver has finished receiving signals.
     */
    InterruptController* const interrupt;
public:
    RemoteControlReceiver( const uint32_t channel );
    virtual ~RemoteControlReceiver();
};

#endif
