#ifndef H_SpiConfig
#define H_SpiConfig

#include <stdint.h>
#include "bits/BitSetRW.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief Configuration (CONF) phase manager.
 */
class SpiConfig
{
private:
    /* data */
public:
    /** Number of SPI_CLK cycles of SPI_CONF state. Can be configured in CONF state. */
    SubValueRW* const configLength;
    /** Full-duplex communication. */
    FlagRW* const fullDuplex;
    /** Set controller in QPI (quad parallel) mode. Both for master mode and slave mode. */
    FlagRW* const modeQPI;
    /** Set controller in OPI (octal parallel) mode. Both for master mode and slave mode. */
    FlagRW* const modeOPI;
    /** Keep SPI CS low when SPI is in DONE phase. */
    FlagRW* const holdCS;
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
