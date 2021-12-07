#ifndef H_SpiOutput
#define H_SpiOutput

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Data output (DOUT) phase manager.
 */
class SpiOutput
{
public:
    /** Enable the send data out phase of an operation. */
    FlagRW* const enable;
    /** The index of last bit in write-data phase. The register value shall be (bit_num-1). */
    SubValueRW* const length;
    /** In write (CMD, ADDRESS and MOSI) phase, 1: big-endian, 0: little-endian. TODO enum */
    FlagRW* const bitOrder;
public:
    SpiOutput( const uint32_t registryBlockOffset );
    virtual ~SpiOutput();
};

#endif
