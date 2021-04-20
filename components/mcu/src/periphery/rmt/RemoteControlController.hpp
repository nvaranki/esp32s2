// Periphery Unit: multi-channel configurable pulse I/O for remote control.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlController
#define H_RemoteControlController

#include <stdint.h>
#include "soc/rmt_caps.h"
#include "bits/FlagRW.hpp"
#include "bits/WordRO.hpp"
#include "RemoteControlChannel.hpp"
#include "RemoteControlMemory.hpp"

class RemoteControlController
{
public:
    static constexpr uint32_t MAX_CHANNEL = SOC_RMT_CHANNELS_NUM;
public:
    /** Module clock control */
    FlagRW* const clock;
    /** Module reset control */
    FlagRW* const reset;
    /** The registers clock gate enable signal of RMT module. 
     * 1: the registers can be read and written by application. 
     * 0: the registers can not be read or written by application 
     */
    FlagRW* const enable;
    /** enable multiple channels to start sending data simultaneously. */
    FlagRW* const sendAltogether;
    /** FIFO RAM for signal I/O */
    RemoteControlMemory* const memory;
    /** Hardware version */
    WordRO* const version;
private:
    RemoteControlChannel* cnl[MAX_CHANNEL];
public:
    RemoteControlController();
    virtual ~RemoteControlController();
public:
    RemoteControlChannel* getChannel( const uint32_t i );
};

#endif
