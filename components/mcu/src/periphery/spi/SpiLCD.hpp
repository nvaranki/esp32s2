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
public:
    SpiLCD( const uint32_t registryBlockOffset );
    virtual ~SpiLCD();
};

#endif
