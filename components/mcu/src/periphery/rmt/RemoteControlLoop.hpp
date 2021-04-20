// Continuous send mode management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlLoop
#define H_RemoteControlLoop

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class RemoteControlLoop
{
public:
    /** enable loop counting */
    FlagRW* const enable;
    /** maximum loop times */
    SubValueRW* const repeat;
    /** reset loop counting when continuous transmission mode is valid */
    FlagRW* const reset;
public:
    RemoteControlLoop( const uint32_t channel );
    virtual ~RemoteControlLoop();
};

#endif
