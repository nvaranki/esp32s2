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
    /** virtual address (in 32 bit words) of entry pair to be transmitted when to fire interrupt;
     * every turn around within a block in wrap mode or cross blocks reading counts towards checked value
     */
    SubValueRW* const threshold;
    /** threshold interrupt status and control;
     * Triggered when the amount of data the transmitter has sent matches the value of "threshold" property.
     */
    InterruptController* const interrupt;
public:
    RemoteControlLimit( const uint32_t channel );
    virtual ~RemoteControlLimit();
};

#endif
