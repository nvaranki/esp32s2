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
private:
public:
public:
    SpiDMA( const uint32_t registryBlockOffset );
    virtual ~SpiDMA();
};

#endif
