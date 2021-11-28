#ifndef H_SerialPeripheralInterface
#define H_SerialPeripheralInterface

#include <stdint.h>
#include "SpiController.hpp"

class SerialPeripheralInterface
{
public:
    static constexpr uint32_t MAX_UNIT = 4;//TODO SOC_PCNT_UNIT_NUM;
private:
    static constexpr uint32_t BaseAddress[MAX_UNIT] = 
    {
        0x00003000, // SPI0
        0x00002000, // SPI1
        0x00024000, // SPI2, GP-SPI2, HSPI, FSPI, ...
        0x00025000  // SPI3, GP-SPI3, VSPI, HSPI, ...
//     ,0x00037000  // SPI4 (declared in API)
//     ,0x00038000  // SPI5 (supposedly)
    };
private:
    SpiController* ctrl[MAX_UNIT];
public:
    SerialPeripheralInterface();
    virtual ~SerialPeripheralInterface();
public:
    /**
     * @brief Obtains SPI controller
     * @htmlonly 
     * <p>All SPI controllers can be used to communicate with external SPI devices:
     * <ul>
     * <li>SPI0 is only used by the CPU cache or EDMA for external RAM and flash</li>
     * <li> SPI1 can be used by the CPU to access various external flash.</li>
     * <li>GP-SPI2 is a general purpose SPI controller with its own DMA channel. 
     *      GP-SPI2 works as either a master or a slave.</li>
     * <li>GP-SPI3 is a general purpose SPI controller, but it shares a DMA channel with ADC 
     *      and DAC modules. GP-SPI3 works as either a master or a slave.</li>
     * </ul>
     * </p>
     * @endhtmlonly
     * 
     * @param i index of controller.
     * @return SPI controller.
     */
    SpiController* getUnit( const uint32_t i );
};

#endif
