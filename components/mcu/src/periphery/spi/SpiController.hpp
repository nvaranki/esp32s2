#ifndef H_SpiController
#define H_SpiController

#include <stdint.h>
#include "SpiAddress.hpp"
#include "SpiCommand.hpp"
#include "SpiConfig.hpp"
#include "SpiPrepare.hpp"
#include "SpiDMA.hpp"
#include "SpiDone.hpp"
#include "SpiDummy.hpp"
#include "SpiIdle.hpp"
#include "SpiInput.hpp"
#include "SpiLCD.hpp"
#include "SpiOutput.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

class SpiController
{
private:
public:
    uint32_t const id;
    /** An operation will be triggered when the bit is set. The bit will be cleared once the operation done. */
    FlagRW* const userDefinedCommand;
    /** Phases of SPI state machine. */
    class Phase
    {
    public:
        /** Configuration phase #0 */
        SpiIdle* const idle;
        /** Configuration phase #1 */
        SpiConfig* const config;
        /** Prepare phase #2 */
        SpiPrepare* const prepare;
        /** Command phase #3 */
        SpiCommand* const command;
        /** Address phase #4 */
        SpiAddress* const address;
        /** Dummy phase #5 */
        SpiDummy* const dummy;
        /** Data output phase #6.1 */
        SpiOutput* const output;
        /** Data input phase #6.2 */
        SpiInput* const input;
        /** Done phase #7 */
        SpiDone* const done;
    public:
        Phase( const uint32_t registryBlockOffset );
        virtual ~Phase();
    }
    const phase;
    /** DMA manager */
    SpiDMA* const dma;
    /** LCD manager */
    SpiLCD* const lcd;
    /** SPI work mode. 1: slave mode 0: master mode. */
    FlagRW* const slave;
    /** Full-duplex communication. */
    FlagRW* const fullDuplex;
    /** Version control register. */
    SubValueRW* const version;
public:
    SpiController( const uint32_t i, const uint32_t registryBlockOffset );
    virtual ~SpiController();
};

#endif
