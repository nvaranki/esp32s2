#ifndef H_SpiDummy
#define H_SpiDummy

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Done (DONE) phase manager.
 */
class SpiDummy
{
public:
    /** Enable the dummy phase of an operation. */
    FlagRW* const enable;
    /** The length in spi_clk cycles of dummy phase. The register value shall be (cycle_num-1). */
    SubValueRW* const length;
    /** Signal level of spi to output. */
    FlagRW* const level;


    /** Disable SPI clock in DUMMY phase. */
    FlagRW* const disableDummyClock;
    /** Keep SPI hold in DUMMY phase. */
    FlagRO* const hold;
    /** Keep SPI CS low when SPI is in DONE phase. */
    FlagRO* const holdCS;
public:
    SpiDummy( const uint32_t registryBlockOffset );
    virtual ~SpiDummy();
};

#endif
