// Idle mode management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlIdle
#define H_RemoteControlIdle

#include <stdint.h>
#include "bits/FlagRW.hpp"

class RemoteControlIdle
{
public:
    /** output enable for the channel when in idle state */
    FlagRW* const output;
    /** level of output signal when in idle state */
    FlagRW* const level;
public:
    RemoteControlIdle( const uint32_t channel );
    virtual ~RemoteControlIdle();
};

#endif
