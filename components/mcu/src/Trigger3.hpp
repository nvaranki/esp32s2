// Triggered on-off switch, amended with indicator of current state.
//
// © 2021 Nikolai Varankine

#ifndef H_Trigger3
#define H_Trigger3

#include "Trigger2.hpp"
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"

class Trigger3 : public Trigger2
{
public:
    FlagRO* const state; //!< true: on, false: off
public:
    Trigger3( FlagRO* const state, FlagRW* const on, FlagRW* const off ) :
        Trigger2( on, off ), state( state ) {};
    virtual ~Trigger3() {};
};

#endif
