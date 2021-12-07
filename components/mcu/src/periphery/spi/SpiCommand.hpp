#ifndef H_SpiCommand
#define H_SpiCommand

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Command (CMD) phase manager.
 */
class SpiCommand
{
public:
    /** Enable command phase of an operation. */
    FlagRW* const enable;
    /** The index of last bit of command phase. The register value shall be (bit_num-1). */
    SubValueRW* const length;
    /** In write (CMD, ADDRESS and MOSI) phase, 1: big-endian, 0: little-endian. TODO enum */
    FlagRW* const bitOrder;
    /** The value of command. */
    SubValueRW* const value;
public:
    SpiCommand( const uint32_t registryBlockOffset );
    virtual ~SpiCommand();
};

#endif
