// Transmission marker, part of remote control transmitter.
// Used in continuous mode as indicator of the moment to refresh data.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlLimit
#define H_RemoteControlLimit

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"
#include "InterruptController.hpp"

class RemoteControlLimit
{
public:
    /** number of transmitted signal entries (level and duration) when to fire interrupt */
    SubValueRW* const entries;
    /** threshold interrupt status and control;
     * Triggered when the amount of data the transmitter has sent matches the value of "entries" property.
     */
    InterruptController* const interrupt;
public:
    RemoteControlLimit( const uint32_t channel );
    virtual ~RemoteControlLimit();
};

#endif
