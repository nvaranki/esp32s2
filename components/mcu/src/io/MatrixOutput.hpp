// Output multiplexor of GPIO Matrix.
//
// © 2021 Nikolai Varankine

#ifndef H_MatrixOutput
#define H_MatrixOutput

#include <stdint.h>
#include "esp32s2/rom/gpio.h"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class MatrixOutput
{
public:
    /** Special index of perpheral output, when MatrixBuffer serves as a "peripheral output" to GPIO pin  */
    static constexpr uint32_t PERIPHERY_BUFFER = 0x100; 
    /** index of perpheral output, see "Table 22: GPIO Matrix" */
    SubValueRW* const periphery;
    class Enable
    {
    private:
        /** Source of enable signal for the channel. 0: peripheral, 1: register tODO  */
        FlagRW* const select;
    public:
        /** Invert incoming enable signal */
        FlagRW* const invert;
    public:
        Enable( const uint8_t i );
        virtual ~Enable();
    public:
        enum class Source : uint32_t
        {
            PERIPHERY = 0,
            GPIO      = 1,
        };
        Source getSource() const { return (Source) select->get(); };
        void setSource( Source s ) const { select->set( static_cast<uint32_t>( s ) ); };
    }
    const enable;
    /** Invert output of the channel */
    FlagRW* const invert;
    /** Index of the perpheral input channel */
    const uint8_t gpio;
public:
    /** @param gpio GPIO pad number to put signal to, [0;53] */
    MatrixOutput( const uint8_t gpio );
    virtual ~MatrixOutput();
};

#endif
