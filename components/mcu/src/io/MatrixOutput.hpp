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
    enum class Constant : uint32_t
    {
        /** Number of internal pad that holds constantly high value */
        HIGH = GPIO_FUNC_IN_HIGH,
        /** Number of internal pad that holds constantly low value */
        LOW  = GPIO_FUNC_IN_LOW,
    };
public:
    /** Peripheral output to get signal from, [0;255] or 256 ... TODO */
    SubValueRW* const source;
    /** 1: route signals via selected GPIO matrix pad; 
        0: connect signals directly to peripheral configured in IO_MUX */
    class Enable
    {
    public:
        /** Source of enable signal for the channel. 0: peripheral, 1: register tODO  */
        FlagRW* const select;
        /** Invert incoming enable signal */
        FlagRW* const invert;
    public:
        Enable( const size_t n );
        virtual ~Enable();
    }
    const enable;
    /** Invert output of the channel */
    FlagRW* const invert;
public:
    MatrixOutput( const size_t n );
    virtual ~MatrixOutput();
};

#endif
