// An I/O channel, part of remote control controller.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlChannel
#define H_RemoteControlChannel

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
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
    /** use carrier modulation */
    FlagRW* const carrier;
    /** FSM status */
    SubValueRO* const status;
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
