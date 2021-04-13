// A set of triggers to check pulse counter value. 
// Interrupt control included.
//
// © 2021 Nikolai Varankine

#ifndef H_PulseCountComparator
#define H_PulseCountComparator

#include <cstddef>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "InterruptController.hpp"

class PulseCountComparator
{
public:
    typedef class LimitComparator
    {
    public:
        /** On/off comparator */
        FlagRW* const enable;
        /** A value when comparator fires. */
        SubValueRW* const threshold;
        /** Event indicator, when current pulse counter equals to threshold */
        FlagRO* const event;
    public:
        LimitComparator( const size_t unit, bool high );
        virtual ~LimitComparator();
    }
    LimitComparator_t;
    typedef class ThresholdComparator
    {
    public:
        /** On/off comparator */
        FlagRW* const enable;
        /** A value when comparator fires. */
        SubValueRW* const threshold;
        /** Event indicator, when current pulse counter equals to threshold */
        FlagRO* const event;
    public:
        ThresholdComparator( const size_t unit, bool id );
        virtual ~ThresholdComparator();
    }
    ThresholdComparator_t;
    typedef class ZeroComparator
    {
    public:
        /** On/off comparator */
        FlagRW* const enable;
        // A threshold value when comparator fires is hardware 0.
        /** Event indicator, when current pulse counter equals to 0 */
        FlagRO* const event;
    public:
        ZeroComparator( const size_t unit );
        virtual ~ZeroComparator();
    }
    ZeroComparator_t;
public:
    LimitComparator_t const low;
    LimitComparator_t const high;
    ThresholdComparator_t const thr0;
    ThresholdComparator_t const thr1;
    ZeroComparator_t const zero;
    class Interrupt : public InterruptController
    {
    public:
        Interrupt( const size_t unit );
        virtual ~Interrupt();
    }
    const interrupt;
public:
    PulseCountComparator( const size_t unit );
    virtual ~PulseCountComparator();
};

#endif
