#include <stdint.h>
#include "WatchDogMain.hpp"

class TimerGeneral
{
public:
    const uint8_t id;
public:
    TimerGeneral( const uint8_t tg, const uint8_t id );
    virtual ~TimerGeneral();
};
