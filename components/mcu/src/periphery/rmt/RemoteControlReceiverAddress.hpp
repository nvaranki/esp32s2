// Memory management, part of remote control receiver.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlReceiverAddress
#define H_RemoteControlReceiverAddress

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"

/** RAM address to write received signal */
class RemoteControlReceiverAddress
{
public:
    /** current memory address offset when channel writes RAM via APB bus */
    SubValueRO* const fifo;
    /** current memory address offset (in 32 bit words) when channel writes RAM directly */
    SubValueRO* const ram;
    /** reset RAM write address */
    FlagWO* const reset;
    /** offset address is out of memory size */
    FlagRO* const error;
    /** more data received than the memory allows */
    FlagRO* const full;
public:
    RemoteControlReceiverAddress( const uint32_t channel );
    virtual ~RemoteControlReceiverAddress();
};

#endif
