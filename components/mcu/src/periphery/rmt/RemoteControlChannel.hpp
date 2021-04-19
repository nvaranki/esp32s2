// An I/O channel of remote control controller.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlChannel
#define H_RemoteControlChannel

#include <cstddef>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "RemoteControlReceiver.hpp"
#include "RemoteControlTransmitter.hpp"

class RemoteControlChannel
{
private:
    RemoteControlReceiver receiver;
    RemoteControlTransmitter transmitter;
public:
    /** enable carrier modulation */
    FlagRW* const carrier;//TODO trans
public:
    class Clock
    {
    private:
        /** select a base clock */
        FlagRW* const source;
    public:
        SubValueRW* const divider;
    public:
        Clock( const size_t channel );
        virtual ~Clock();
    public:
        enum class Source
        {
            /** External reference ticks */
            REF = 0,
            /** APB bus clock */
            APB = 1,
        };
        Source getSource() const { return (Source) source->get(); };
        void setSource( const Source s ) const { source->set( static_cast<uint32_t>( s ) ); };
    }
    /*TODO const*/ clock;
    class Memory
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
        enum class Owner : uint32_t
        {
            TRANSMITTER = 0,
            RECIVER     = 1,
        };
    public:
        Memory( const size_t channel );
        virtual ~Memory();
    public:
        /** @return Number of memory blocks allocated to the channel */
        uint32_t getAllocated() const;
        /** @param n Number of memory blocks to allocate for the channel; no more than reminder of the pool. */
        void setAllocated( const uint32_t n );
        Owner getOwner() const { return (Owner) owner->get(); };
        void setOwner( const Owner o ) const { owner->set( static_cast<uint32_t>( o ) ); };
    }
    /*TODO const*/ memory;
public:
    /** FSM status */
    SubValueRO* const status;
    /** index of the channel */
    const size_t id;
public:
    RemoteControlChannel( const size_t i );
    virtual ~RemoteControlChannel();
public:
    RemoteControlReceiver* getReceiver() { return &receiver; };
    RemoteControlTransmitter* getTransmitter() { return &transmitter; };
};

#endif
