#ifndef H_SpiAddress
#define H_SpiAddress

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Address (ADDR) phase manager.
 */
class SpiAddress
{
public:
    /** Enable the address phase of an operation. */
    FlagRW* const enable;
    /** The length in bits of address phase. The register value shall be (bit_num-1). */
    SubValueRW* const length;
    /** In write (CMD, ADDRESS and MOSI) phase, 1: big-endian, 0: little-endian. TODO enum */
    FlagRW* const bitOrder;
    /** Address to slave. */
    WordRW* const value;
public:
    SpiAddress( const uint32_t registryBlockOffset );
    virtual ~SpiAddress();
};

#endif
