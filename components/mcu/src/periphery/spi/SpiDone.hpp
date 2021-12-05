#ifndef H_SpiDone
#define H_SpiDone

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Done (DONE) phase manager.
 */
class SpiDone
{
private:
public:
    /** Keep SPI CS low when SPI is in DONE phase. */
    FlagRW* const holdCS;
    /** delay cycles of cs pin by spi clock */
    SubValueRW* const length;
    
public:
    SpiDone( const uint32_t registryBlockOffset );
    virtual ~SpiDone();
};

#endif
