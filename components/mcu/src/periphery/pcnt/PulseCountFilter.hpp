// Input pulse filter.
// Applies to both channels.
//
// © 2021 Nikolai Varankine

#ifndef H_PulseCountFilter
#define H_PulseCountFilter

#include <cstddef>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class PulseCountFilter
{
public:
    /** Apply or bypass filter */
    FlagRW* const enable;
    /** The maximum threshold, in APB clock cycles, for the filter.
        Any input pulses with width less than this one will be ignored. */
    SubValueRW* const threshold;
public:
    PulseCountFilter( const size_t unit );
    virtual ~PulseCountFilter();
};

#endif
