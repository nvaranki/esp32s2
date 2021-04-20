// General manager of entire pulse data memory, part of remote control controller.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlMemory
#define H_RemoteControlMemory

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "Trigger2.hpp"

class RemoteControlMemory
{
public:
    FlagRW* const clock;
    /** repetitive usage of data */
    FlagRW* const wrap;
    /** 1: Access memory directly; 0: Access memory via APB FIFO */
    FlagRW* const direct;
    Trigger2 power;
public:
    RemoteControlMemory();
    virtual ~RemoteControlMemory();
};

#endif
