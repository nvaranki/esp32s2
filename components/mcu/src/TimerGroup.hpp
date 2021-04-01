#include <stdint.h>
#include "TimerGeneral.hpp"
#include "WatchDogMain.hpp"

class TimerGroup
{
public:
    TimerGeneral* const t0;
    TimerGeneral* const t1;
    WatchDogMain* const wd;
    const uint8_t id;
public:
    TimerGroup( const uint8_t id );
    virtual ~TimerGroup();
};
