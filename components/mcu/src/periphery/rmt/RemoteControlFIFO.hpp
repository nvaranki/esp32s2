// Sequential accessor to pulse data memory; part of remote control channel memory.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlFIFO
#define H_RemoteControlFIFO

#include <stdint.h>
#include "soc/rmt_struct.h"
#include "bits/FlagRO.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/WordRW.hpp"

class RemoteControlFIFO
{
private:
    SubValueRO* const allocated;
public:
    /** 
     * data I/O gateway for APB FIFO; 
     * it puts into or reads from the same RAM area 
     */
    WordRW* const data;
    /** current FIFO address offset (in 32 bit words) when channel reads RAM via APB bus */
    SubValueRO* const addressRead;
    /** current FIFO address offset (in 32 bit words) when channel writes RAM via APB bus */
    SubValueRO* const addressWrite;
    /** Set this bit to reset I/O address to the beginning of block, used by FIFO to access RAM */
    FlagWO* const reset;
    /** offset address is out of memory size */
    FlagRO* const overflowRead;
    /** offset address is out of memory size */
    FlagRO* const overflowWrite;
public:
    RemoteControlFIFO( const uint32_t channel, SubValueRO* const allocated );
    virtual ~RemoteControlFIFO();
};

#endif
