// An I/O channel, part of remote control controller.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlChannel
#define H_RemoteControlChannel

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/WordRW.hpp"
#include "InterruptController.hpp"
#include "RemoteControlClock.hpp"
#include "RemoteControlChannelMemory.hpp"
#include "RemoteControlReceiver.hpp"
#include "RemoteControlTransmitter.hpp"

class RemoteControlChannel
{
private: // these modules unlikely are used together
    /** Subchannel to get signal in */
    RemoteControlReceiver* receiver;
    /** Subchannel to put signal out */
    RemoteControlTransmitter* transmitter;
    /** Clock manager */
    RemoteControlClock* clock;
    /** Memory manager */
    RemoteControlChannelMemory* memory;
public:
    /** data I/O via APB FIFO */
    WordRW* const data;
    /** use carrier modulation */
    FlagRW* const carrier;
    /** FSM status */
    SubValueRO* const status;
    /** I/O interrupt status and control;
     * Triggered when channel n does not read or write data correctly. For example, if the
     * transmitter still tries to read data from RAM when the RAM is empty, or the receiver 
     * still tries to write data into RAM when the RAM is full, this interrupt will be triggered.
     */
    InterruptController* const interrupt;
    /** index of the channel */
    const uint32_t id;
public:
    RemoteControlChannel( const uint32_t i );
    virtual ~RemoteControlChannel();
public:
    RemoteControlReceiver* getReceiver();
    RemoteControlTransmitter* getTransmitter();
    RemoteControlClock* getClock(); 
    RemoteControlChannelMemory* getMemory(); 
};

#endif
