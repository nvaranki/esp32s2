#ifndef H_ChipSelect
#define H_ChipSelect

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

/** Chip Select signal manager. */
class ChipSelect
{
public:
    /** Delay in prepare phase between initiating CS signal front raise/fall 
     *  and moment of data validity.
     *  0: setup time is 0.5 x one cycle of SPI_CLK.
     *  1: setup time is (SPI_CS_SETUP_TIME + 1.5) x one cycle of SPI_CLK.
     */
    FlagRW* const setupMode;
    /** Delay in prepare phase between initiating CS signal front raise/fall 
     *  and moment of data validity, in SPI_CLK ticks. */
    SubValueRW* const setupTime;
    /** Delay in done  phase between moment the data are not interested and 
     *  finalizing CS signal front raise/fall.
     *  0: setup time is 0.5 x one cycle of SPI_CLK.
     *  1: setup time is (SPI_CS_HOLD_TIME + 1.5) x one cycle of SPI_CLK.
     */
    FlagRW* const holdMode;
    /** Delay in done  phase between moment the data are not interested and 
     *  finalizing CS signal front raise/fall, in SPI_CLK ticks. 
     */
    SubValueRW* const holdTime;
    /** CS signal is delayed by spi clock cycles. */
    SubValueRW* const holdDelay;
    /** cs signal is delayed by spi_clk ticks. 
     * 0: zero;
     * 1: if spi_ck_out_edge or spi_ck_i_edge is set 1 delayed by half cycle  else delayed by one cycle;
     * 2: if spi_ck_out_edge or spi_ck_i_edge is set 1 delayed by one cycle  else delayed by half cycle;
     * 3: delayed one cycle. */
    SubValueRW* const delayMode;
    /** CS signal is delayed by system clock cycles. */
    SubValueRW* const delayTime;
    /** CS line keep low. */
    FlagRW* const keepActive;
    /** CS input polarity (slave mode). 1: inv  0: not change. */
    FlagRW* const inputPolarity;
    /** CS output polarity (master mode). The value is equivalent to spi_cs ^ spi_master_cs_pol. */
    SubValueRW* const outputPolarity;
public:
    ChipSelect( const uint32_t registryBlockOffset );
    virtual ~ChipSelect();
};

#endif
