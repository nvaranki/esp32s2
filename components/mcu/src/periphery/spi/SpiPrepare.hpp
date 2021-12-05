#ifndef H_SpiPrepare
#define H_SpiPrepare

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Prepare (PREP) phase manager.
 */
class SpiPrepare
{
public:
    /** Enable prepare phase of an operation. */
    FlagRW* const enable;
    /** Duration of prepare phase in spi clock cycles (value+1). */
    SubValueRW* const duration;
public:
    SpiPrepare( const uint32_t registryBlockOffset );
    virtual ~SpiPrepare();
};

#endif
