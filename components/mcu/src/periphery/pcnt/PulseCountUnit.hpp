// Pulse counter.
//
// © 2021 Nikolai Varankine

#ifndef H_PulseCountUnit
#define H_PulseCountUnit

#include <cstddef>
#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "PulseCountChannel.hpp"
#include "PulseCountComparator.hpp"
#include "PulseCountFilter.hpp"

class PulseCountUnit
{
public:
    static constexpr size_t MAX_CHANNEL = 2;
    enum class Status : uint32_t //TODO --> ZeroComparator ?
    {
        /** counter decreases from positive to 0 */
        POS_DOWN = 0b00,
        /** counter increases from negative to 0 */
        NEG_UP   = 0b01,
        /** counter is negative */
        NEGATIVE = 0b10,
        /** counter is positive */
        POSITIVE = 0b11,
    };
private:
    /** Current pulse count value */
    SubValueRO* const value;
public:
    /** Set this bit to clear counter */
    FlagRW* const reset;
    /** Set this bit to freeze counter */
    FlagRW* const pause;
private:
    /** Current pulse counter status corresponding to 0. */
    SubValueRO* const status;
    /** Input channels for the counter */
    PulseCountChannel* channel[MAX_CHANNEL];
    /** Optional filter of input signals */
    PulseCountFilter* filter;
    /** Optional comparator of the counter value */
    PulseCountComparator* comparator;
public:
    const size_t id;
public:
    PulseCountUnit( const size_t i );
    virtual ~PulseCountUnit();
public:
    Status getStatus() const;
    int16_t getValue() const;
    PulseCountChannel* getChannel( const size_t i );
    PulseCountFilter* getFilter();
    PulseCountComparator* getComparator();
};

#endif
