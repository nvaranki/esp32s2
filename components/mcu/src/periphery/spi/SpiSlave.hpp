#ifndef H_SpiSlave
#define H_SpiSlave

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

class SpiSlave
{
private:
public:
    /** [31:8]: the address to slave, [7:0]: Reserved. */
    WordRW* const userAddress;
    /** Change polarity of TSCK. 0: TSCK = SPI_CK_I. 1: TSCK = !SPI_CK_I. */
    FlagRW* const polarityTSCK;
    /** Change polarity of RSCK.  0: RSCK = !SPI_CK_I. 1: RSCK = SPI_CK_I. */
    FlagRW* const polarityRSCK;
public:
    SpiSlave( const uint32_t registryBlockOffset );
    virtual ~SpiSlave();
};

#endif
