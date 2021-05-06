// Indexed accessor to pulse data memory; part of remote control channel memory.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlRAM
#define H_RemoteControlRAM

#include <stdint.h>
#include "soc/rmt_struct.h"
#include "bits/FlagRO.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"

class RemoteControlRAM
{
private:
    SubValueRO* const allocated;
    /** 
     * pointer to the beginning of channel's RAM area; 
     * it allows direct data I/O
     */
    volatile uint32_t* const ram; // uint16_t* is not readable
public:
    /** current RAM address offset (in 32 bit words) when channel reads RAM directly 
     * TODO it supposedly has a 1 word cache, so address reading is one word forward
    */
    SubValueRO* const addressRead;
    /** current RAM address offset (in 32 bit words) when channel writes RAM directly */
    SubValueRO* const addressWrite;
    /** reset RAM read address to the beginning of block */
    FlagWO* const resetRead;
    /** reset RAM write address to the beginning of block */
    FlagWO* const resetWrite;
    /** more data to be sent than the memory allows, and the wrap mode is disabled */
    FlagRO* const overflowRead;
    /** more data received than the memory allows */
    FlagRO* const overflowWrite;
public:
    RemoteControlRAM( const uint32_t channel, SubValueRO* const allocated );
    virtual ~RemoteControlRAM();
public:
    /** 
     * Reads entry from RAM in direct access mode.
     * @param i entry index in RAM 
     * @return entry extracted from RAM
     */
    rmt_item16_t getEntry( const uint32_t i );
    /** 
     * Writes entry into RAM in direct access mode.
     * @param i entry index in RAM 
     * @param e entry to save into RAM
     */
    void setEntry( const uint32_t i, const rmt_item16_t& e );
    /** 
     * Reads entry pair from RAM in direct access mode.
     * @param i entry pair index in RAM 
     * @return entry pair extracted from RAM
     */
    rmt_item32_t getEntry2( const uint32_t i );
    /** 
     * Writes entry pair into RAM in direct access mode.
     * @param i entry pair index in RAM 
     * @param e entry pair to save into RAM
     */
    void setEntry2( const uint32_t i, const rmt_item32_t& e );
};

#endif
