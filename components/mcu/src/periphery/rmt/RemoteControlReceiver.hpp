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
    /** RAM address to write inbound signal */
    RemoteControlReceiverAddress* const address;
    /** Input sinal filter */
    RemoteControlFilter* const filter;
    /** Carrier remover configuration */
    RemoteControlReceiverCarrier* const carrier;
    /** allow to receive data */
    FlagRW* const enable;
    /** 
     * Number 0f continuous divided clock cycles to detect idle state (no change on input, no matter 
     * of level 0 or 1). Once an idle state has been detected, the channel places end-marker entry to 
     * memory and generates interrupt "end of receive".
     * TODO if input rather continues, some trash entries were noticed after end-marker.
     */
    SubValueRW* const idle;
    /** 
     * end of receive interrupt status and control;
     * Triggered when the receiver has finished receiving signals.
     */
    InterruptController* const interrupt;
    /** Index of the GPIO Matrix input channel */
    uint8_t const index;
public:
    RemoteControlReceiver( const uint32_t channel );
    virtual ~RemoteControlReceiver();
};

#endif
