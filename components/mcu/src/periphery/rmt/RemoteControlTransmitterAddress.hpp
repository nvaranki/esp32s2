// Memory management, part of remote control transmitter.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlTransmitterAddress
#define H_RemoteControlTransmitterAddress

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"

class RemoteControlTransmitterAddress
{
public:
    /** current memory address offset when channel reads RAM via APB bus */
    SubValueRO* const fifo;
    /** current memory address offset (in 32 bit words) when channel reads RAM directly */
    SubValueRO* const ram;
    /** reset RAM read address */
    FlagWO* const reset;
    /** offset address is out of memory size */
    FlagRO* const error;
    /** more data to be sent than the memory allows, and the wrap mode is disabled */
    FlagRO* const full;
public:
    RemoteControlTransmitterAddress( const uint32_t channel );
    virtual ~RemoteControlTransmitterAddress();
};

#endif
