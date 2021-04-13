// Common set of controls for single interrupt.
//
// © 2021 Nikolai Varankine

#ifndef H_InterruptController
#define H_InterruptController

#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"

class InterruptController
{
public:
    FlagRO* const raw;
    FlagRO* const masked;
    FlagRW* const enable;
    FlagWO* const clear;
public:
    InterruptController( FlagRO* const r, FlagRO* const m, FlagRW* const e, FlagWO* const c );
    virtual ~InterruptController() {}
};

#endif
