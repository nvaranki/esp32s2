#ifndef H_SpiController
#define H_SpiController

#include <stdint.h>
#include "SpiConfig.hpp"
#include "SpiPrepare.hpp"
#include "SpiDone.hpp"
#include "SpiDMA.hpp"
#include "SpiLCD.hpp"
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
    /** Configuration phase */
    SpiConfig* const conf;
    /** Prepare phase */
    SpiPrepare* const prep;
    /** Done phase */
    SpiDone* const done;
    /** DMA manager */
    SpiDMA* const dma;
    /** LCD manager */
    SpiLCD* const lcd;
public:
    SpiController( const uint32_t i, const uint32_t registryBlockOffset );
    virtual ~SpiController();
};

#endif
