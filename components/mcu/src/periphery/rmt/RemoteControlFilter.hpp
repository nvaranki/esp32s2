// Input sinal filter management, part of remote control receiver.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlFilter
#define H_RemoteControlFilter

#include <stdint.h> 
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class RemoteControlFilter
{
public:
    /** filter received data */
    FlagRW* const enable;
    /** ignore the input pulse when its width is less than this value, in APB clock cycles */
    SubValueRW* const threshold;
public:
    RemoteControlFilter( const uint32_t channel );
    virtual ~RemoteControlFilter();
};

#endif
