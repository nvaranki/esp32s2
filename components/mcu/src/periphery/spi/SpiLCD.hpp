#ifndef H_SpiLCD
#define H_SpiLCD

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"

/**
 * @brief LCD manager.
 */
class SpiLCD
{
private:
public:
    /** Enable LCD mode output vsync, hsync, de. */
    FlagRW* const enable;
    /** Horizontal blank front porch of a frame. */
    SubValueRW* const blankFrontPorchH;
    /** Vertical blank front porch of a frame. */
    SubValueRW* const blankFrontPorchV;
    /** Horizontal total width of a frame. */
    SubValueRW* const totalH;
    /** Vertical total height of a frame. */
    SubValueRW* const totalV;
    /** Horizontal active width of a frame. */
    SubValueRW* const activeH;
    /** Vertical active height of a frame. */
    SubValueRW* const activeV;
    /** Idle value of spi_hsync. */
    FlagRW* const idlePolarityH;
    /** Idle value of spi_vsync. */
    FlagRW* const idlePolarityV;
    /** Width of spi_hsync active pulse in a line. */
    SubValueRW* const widthSyncH;
    /** Width of spi_vsync active pulse in a line. */
    SubValueRW* const widthSyncV;
    /** Position of spi_hsync active pulse in a line. */
    SubValueRW* const positionSyncH;
//  /** Position of spi_vsync active pulse in a line. */
//  SubValueRW* const positionSyncV;
    /** Select clock source, and sample output signal FSPIDQS from the master with delays. TODO enum */
    SubValueRW* const modeDQS;
    /** Select clock source, and sample output signal FSPICD from the master with delays. TODO enum */
    SubValueRW* const modeCD;
    /** Select clock source, and sample output signal FSPI_DE from the master with delays. TODO enum */
    SubValueRW* const modeDE;
    /** Select clock source, and sample output signal FSPI_HSYNC from the master with delays. */
    SubValueRW* const modeHS;
    /** Select clock source, and sample output signal FSPI_VSYNC from the master with delays. */
    SubValueRW* const modeVS;
    /** Idle value of SPI_DE. */
    FlagRW* const idlePolarityDE;
    /** Mode of pulse of SPI_HSYNC */
    FlagRW* const blankHS;
    /** Delay to output signal FSPIDQS. */
    SubValueRW* const delayDQS;
    /** Delay to output signal FSPI_CD. */
    SubValueRW* const delayCD;
    /** Delay to output signal FSPI_DE. */
    SubValueRW* const delayDE;
    /** Delay to output signal FSPI_HS. */
    SubValueRW* const delayHS;
    /** Delay to output signal FSPI_VS. */
    SubValueRW* const delayVS;
public:
    SpiLCD( const uint32_t registryBlockOffset );
    virtual ~SpiLCD();
};

#endif
