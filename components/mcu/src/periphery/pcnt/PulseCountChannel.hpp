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
private:
    /** Action by the counter on detection of negative edge in the input signal */
    SubValueRW* const negative;
    /** Action by the counter on detection of positive edge in the input signal */
    SubValueRW* const positive;
    /** Modifier to positive/negative action when the control signal is high */
    SubValueRW* const high;
    /** Modifier to positive/negative action when the control signal is low */
    SubValueRW* const low;
public:
    const size_t unit;
    const size_t id;
public:
    PulseCountChannel( const size_t unit, const size_t i );
    virtual ~PulseCountChannel();
public:
    /** Action by the counter on changed input */
    enum class Edge : uint32_t
    {
        /** No effect on counter */
        IGNORE   = 0,
        /**  Increase the counter by 1 */
        INCREASE = 1,
        /**  Decrease the counter by 1 */
        DECREASE = 2,
    };
    /** Modifier to positive/negative action */
    enum class Extremum : uint32_t
    {
        /** No modification */
        IGNORE  = 0,
        /** Invert behavior (increase -> decrease, decrease -> increase) */
        BOUNCE  = 1,
        /** Inhibit counter modification */
        INHIBIT = 2,
    };
    void setOnNegative( const Edge action ) { negative->set( static_cast<uint32_t>( action ) ); }
    void setOnPositive( const Edge action ) { positive->set( static_cast<uint32_t>( action ) ); }
    void setOnHigh( const Extremum action ) { high->set( static_cast<uint32_t>( action ) ); }
    void setOnLow ( const Extremum action ) { low ->set( static_cast<uint32_t>( action ) ); }
    Edge getOnNegative() { return (Edge) negative->get(); }
    Edge getOnPositive() { return (Edge) positive->get(); }
    Extremum getOnHigh() { return (Extremum) high->get(); }
    Extremum getOnLow () { return (Extremum) low ->get(); }
    enum class Input : size_t
    {
        SIGNAL  = 0,
        CONTROL = 1,
    };
    /** 
     * @param i type of channel input
     * @return index of the GPIO Matrix input channel
    */
    size_t getMatrixIndex( const Input i ) const;
};

#endif
