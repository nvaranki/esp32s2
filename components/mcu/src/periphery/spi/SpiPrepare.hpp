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
private:
public:
    /** Enable SPI CS when SPI is in (PREP) prepare phase. */
    FlagRO* const setupCS;
public:
    SpiPrepare( const uint32_t registryBlockOffset );
    virtual ~SpiPrepare();
};

#endif
