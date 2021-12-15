#ifndef H_SpiDMA
#define H_SpiDMA

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief DMA manager.
 */
class SpiDMA
{
public:
    /** 1: enable DMA controlled RX mode. 0: enable CPU controlled RX mode. */
    FlagRW* const receive;
    /** 1: enable DMA controlled TX mode. 0: enable CPU controlled TX mode. */
    FlagRW* const send;
public:
    SpiDMA( const uint32_t registryBlockOffset );
    virtual ~SpiDMA();
};

#endif
