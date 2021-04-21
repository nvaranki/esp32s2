// Clock management, part of remote control channel.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlClock
#define H_RemoteControlClock

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class RemoteControlClock
{
private:
    /** select a base clock */
    FlagRW* const source;
public:
    /** divider of source clock frequency; value 0f 0 means 256 */
    SubValueRW* const divider;
    /** reset the clock divider */
    FlagRW* const reset;
public:
    RemoteControlClock( const size_t channel );
    virtual ~RemoteControlClock();
public:
    enum class Source
    {
        /** External reference ticks */
        REF = 0,
        /** APB bus clock */
        APB = 1,
    };
    /** @return selected base clock */
    inline Source getSource() const { return (Source) source->get(); };
    /** @param s a base clock to select */
    inline void setSource( const Source s ) const { source->set( static_cast<uint32_t>( s ) ); };
};

#endif
