// Triggered on-off switch.
//
// © 2021 Nikolai Varankine

#ifndef H_Trigger2
#define H_Trigger2

#include "bits/FlagRW.hpp"

class Trigger2
{
public:
    FlagRW* const on;  //!< Having off==false, set it true to force state on
    FlagRW* const off; //!< Having on==false, set it true to force state off
public:
    Trigger2( FlagRW* const on, FlagRW* const off ) : on( on ), off( off ) {};
    virtual ~Trigger2() {};
};

#endif
