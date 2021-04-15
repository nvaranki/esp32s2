// Input pulse channel.
//
// © 2021 Nikolai Varankine

#ifndef H_PulseCountChannel
#define H_PulseCountChannel

#include <cstddef>
#include <stdint.h>
#include "soc/gpio_sig_map.h"
#include "bits/SubValueRW.hpp"

class PulseCountChannel
{
public:
    /** Input of the channel to count pulses from. */
    class Signal
    {
    private:
        /** Action by the counter on detection of negative edge in the input signal */
        SubValueRW* const negative;
        /** Action by the counter on detection of positive edge in the input signal */
        SubValueRW* const positive;
    public:
        /** Index of the GPIO Matrix input channel */
        size_t const index;
    public:
        Signal( const size_t unit, const size_t channel );
        virtual ~Signal();
    public:
        /** Action by the counter on changed input */
        enum class Action : uint32_t
        {
            /** No effect on counter */
            IGNORE   = 0,
            /**  Increase the counter by 1 */
            INCREASE = 1,
            /**  Decrease the counter by 1 */
            DECREASE = 2,
        };
        void setOnNegative( const Action action ) { negative->set( static_cast<uint32_t>( action ) ); }
        void setOnPositive( const Action action ) { positive->set( static_cast<uint32_t>( action ) ); }
        Action getOnNegative() { return (Action) negative->get(); }
        Action getOnPositive() { return (Action) positive->get(); }
    }
    /*TODO const*/ signal;
    
    /** Input of the channel to control counting process on signal input. */
    class Control
    {
    private:
        /** Modifier to positive/negative action when the control signal is high */
        SubValueRW* const high;
        /** Modifier to positive/negative action when the control signal is low */
        SubValueRW* const low;
    public:
        /** Index of the GPIO Matrix input channel */
        size_t const index;
    public:
        Control( const size_t unit, const size_t channel );
        virtual ~Control();
    public:
        /** Modifier to positive/negative action over signal */
        enum class Action : uint32_t
        {
            /** No modification */
            IGNORE  = 0,
            /** Invert behavior (increase -> decrease, decrease -> increase) */
            BOUNCE  = 1,
            /** Inhibit counter modification */
            INHIBIT = 2,
        };
        void setOnHigh( const Action action ) { high->set( static_cast<uint32_t>( action ) ); }
        void setOnLow ( const Action action ) { low ->set( static_cast<uint32_t>( action ) ); }
        Action getOnHigh() { return (Action) high->get(); }
        Action getOnLow () { return (Action) low ->get(); }
    }
    /*TODO const*/ control;

    /** Unit index within controller */
    const size_t unit;
    /** Channel index within unit */
    const size_t id;
public:
    PulseCountChannel( const size_t unit, const size_t channel );
    virtual ~PulseCountChannel();
};

#endif
