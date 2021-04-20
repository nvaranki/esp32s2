// Pulse data memory access management; part of remote control channel.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlChannelMemory
#define H_RemoteControlChannelMemory

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class RemoteControlChannelMemory
{
public:
    /** Maximum number of memory blocks allocated to channel, including reminder of the pool. */
    const uint32_t allocatedMax;
private:
    /** Number of memory blocks allocated to channel; no more than reminder of the pool. */
    SubValueRW* const allocated;
    /** Ownership of channel’s RAM block. */
    FlagRW* const owner;
    /** Ownership of memory block is violated */
    FlagRO* const error;
public:
    RemoteControlChannelMemory( const uint32_t channel );
    virtual ~RemoteControlChannelMemory();
public:
    /** Performer of memory I/O inside remote control controller */
    enum class Owner : uint32_t
    {
        TRANSMITTER = 0,
        RECIVER     = 1,
    };
    /** @return Number of memory blocks allocated to the channel */
    uint32_t getAllocated() const;
    /** @param n Number of memory blocks to allocate for the channel; no more than reminder of the pool. */
    void setAllocated( const uint32_t n );
    /** @return current ownership of channel’s RAM block. */
    inline Owner getOwner() const { return (Owner) owner->get(); };
    /** @param o Ownership of channel’s RAM block. */
    inline void setOwner( const Owner o ) const { owner->set( static_cast<uint32_t>( o ) ); };
};

#endif