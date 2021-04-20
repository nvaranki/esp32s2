// Memory management, part of remote control receiver.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlReceiverAddress
#define H_RemoteControlReceiverAddress

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"

/** RAM address to write received signal */
class RemoteControlReceiverAddress
{
public:
    /** memory address offset when channel writes RAM via APB bus */
    SubValueRW* const offset;
    /** current memory address offset when channel writes RAM via APB bus */
    SubValueRO* const current;
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
