#ifndef H_SpiInput
#define H_SpiInput

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Data input (DIN) phase manager.
 */
class SpiInput
{
public:
    /** Enable the receive data in phase of an operation*/
    FlagRW* const enable;
    /** The index of last bit in read-data phase. The register value shall be (bit_num-1). */
    SubValueRW* const length;
    /** In read (MISO) phase, 1: big-endian, 0: little-endian. TODO enum */
    FlagRW* const bitOrder;
public:
    SpiInput( const uint32_t registryBlockOffset );
    virtual ~SpiInput();
};

#endif
