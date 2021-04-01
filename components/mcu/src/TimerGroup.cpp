#include "TimerGroup.hpp"

TimerGroup::TimerGroup( const uint8_t id ) :
    t0( new TimerGeneral( id, 0 ) ),
    t1( new TimerGeneral( id, 1 ) ),
    wd( new WatchDogMain( id ) ),
    id( id )
{
}
    
TimerGroup::~TimerGroup()
{
    delete t0;
    delete t1;
    delete wd;
}
