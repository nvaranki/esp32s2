#ifndef H_SpiIdle
#define H_SpiIdle

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Idle (IDLE) phase manager.
 */
class SpiIdle
{
public:
    /** Enable user defined command.  
     * An operation (walk through state machine graph) will be triggered when
     * the bit is set.  The bit will be cleared once the operation is done. */
    FlagRW* const enable; //TODO "start" ?
public:
    SpiIdle( const uint32_t registryBlockOffset );
    virtual ~SpiIdle();
};

#endif
