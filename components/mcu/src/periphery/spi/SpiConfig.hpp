#ifndef H_SpiConfig
#define H_SpiConfig

#include <stdint.h>
#include "bits/BitSetRW.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Configuration (DMA CONF) phase manager.
 * Used in seg-trans operations only.
 */
class SpiConfig
{
public:
    /** Enable first configuration phase of an operation,
        which means seg-trans will start. */
    FlagRW* const enable;
    /** Number of SPI_CLK cycles in configuration state. */
    SubValueRW* const length;
    /** Enable next configuration phase of an operation,
        which means seg-trans will continue. */
    FlagRW* const next;

    /** Full-duplex communication. */
    FlagRW* const fullDuplex;
    /** Set controller in QPI (quad parallel) mode. Both for master mode and slave mode. */
    FlagRW* const modeQPI;
    /** Set controller in OPI (octal parallel) mode. Both for master mode and slave mode. */
    FlagRW* const modeOPI;
    /** Enable SPI CS when SPI is in (PREP) prepare phase. */
    FlagRW* const setupCS;
    /** Together with SPI_DOUT_MODE is used to set MOSI signal delay mode. */
    FlagRW* const delayMOSI;
private:
    /** In read (MISO) phase, 1: big-endian, 0: little-endian. TODO enum */
    FlagRW* const bitOrderMISO;
    /** In write (CMD, ADDRESS and MOSI) phase, 1: big-endian, 0: little-endian. TODO enum */
    FlagRW* const bitOrderMOSI;
    /** In write operations, mode of read-data phase: 1,2,4,8-bit. TODO enum */
    BitSetRW* const widthMISO;
    
    /** Byte view on data buffer */
    uint8_t* const data8;
    /** Double byte view on data buffer */
    uint16_t* const data16;
    /** Quad byte view on data buffer */
    uint32_t* const data32;
    /** Location and size of MOSI data buffer in CPU controlled mode:
     * 0: full 72 bytes buffer;
     * 1: short 40 bytes buffer (tail of full buffer).
     */
    FlagRW* const dataAreaMOSI;
    /** Location and size of MISO data buffer in CPU controlled mode: 
     * 0: full 72 bytes buffer;
     * 1: short 40 bytes buffer (tail of full buffer).
     */
    FlagRW* const dataAreaMISO;

public:
    class Delay
    {
    public:
        class Descr
        {
        public:
            SubValueRW* const edge;
            SubValueRW* const value;
        public:
            Descr( const uint32_t aEdge,  const uint32_t mEdge,  const uint32_t sEdge, 
                   const uint32_t aValue, const uint32_t mValue, const uint32_t sValue );
            virtual ~Descr();
        }
        const inp, out;
    public:
        Delay( const uint32_t registryBlockOffset, const uint32_t i );
        virtual ~Delay();
    }
    const delay[8]; //!< FSPID, FSPIQ, FSPIWP, FSPIHD, FSPIIO4, ..., FSPIIO7

public:
    SpiConfig( const uint32_t registryBlockOffset );
    virtual ~SpiConfig();
};

#endif
