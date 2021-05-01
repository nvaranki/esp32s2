// Pulse data memory access management; part of remote control channel.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlChannelMemory
#define H_RemoteControlChannelMemory

#include <stdint.h>
#include "soc/rmt_struct.h"
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

class RemoteControlChannelMemory
{
public:
    /** Maximum number of memory blocks allocated to channel, including reminder of the pool. */
    const uint32_t allocatedMax;
    /** Number of memory blocks allocated to channel; no more than reminder of the pool. */
    SubValueRO* const allocated;
    /** repetitive usage of data */
private:
    /** Ownership of channel’s RAM block. */
    FlagRW* const owner;
public:
    /** Ownership of memory block is violated */
    FlagRO* const error;
    /** 
     * data I/O gateway for APB FIFO; 
     * it puts into or reads from the same RAM area 
     */
    WordRW* const fifo;
    /** Set this bit to reset I/O address used by FIFO to access RAM */
    FlagWO* const reset;
private:
    /** 
     * pointer to the beginning of channel's RAM area; 
     * it allows direct data I/O
     */
    volatile uint32_t* const ram; // uint16_t* is not readable
public:
    RemoteControlChannelMemory( const uint32_t channel );
    virtual ~RemoteControlChannelMemory();
public:
    /** Performer of memory I/O inside remote control controller */
    enum class Owner : uint32_t
    {
        /**
         * Transmitter memory read mode.
         * It is safe to perform software I/O with memory.
         */
        SW_TRANSMITTER = 0,
        /**
         * Receiver memory write mode.
         * It is NOT safe to perform software I/O with memory.
         */
        HW_RECEIVER    = 1,
    };
    /** @param n Number of memory blocks to allocate for the channel; no more than reminder of the pool. */
    void setAllocated( const uint32_t n );
    /** @return current ownership of channel’s RAM block. */
    inline Owner getOwner() const { return (Owner) owner->get(); };
    /** @param o Ownership of channel’s RAM block. */
    inline void setOwner( const Owner o ) const { owner->set( static_cast<uint32_t>( o ) ); };
    /** 
     * Reads entry from RAM in direct access mode.
     * @param i pulse entry index in RAM 
     * @return pulse entry extracted from RAM
     */
    rmt_item16_t getEntry( const uint32_t i );
    /** 
     * Writes entry into RAM in direct access mode.
     * @param i pulse entry index in RAM 
     * @param e pulse entry to save into RAM
     */
    void setEntry( const uint32_t i, const rmt_item16_t& e );
    /** 
     * @param level signal value
     * @param duration signalduration, in divided APB ticks
     * @return pulse entry
     */
    static rmt_item16_t entry( const bool level, const uint16_t duration );
    /** 
     * @param e0 first pulse entry
     * @param e1 second pulse entry
     * @return pair of pulse entries to fit FIFO gateway width
     */
    static rmt_item32_t entry2( const rmt_item16_t& e0, const rmt_item16_t& e1 );
    /** 
     * End of entries marker; it is required when number of entries in RAM is less 
     * than capacity of allocated memory blocks 
     */
    static const rmt_item16_t ENTRY_END;
};

#endif