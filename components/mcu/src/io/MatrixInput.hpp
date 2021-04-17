// Input multiplexor of GPIO Matrix.
//
// © 2021 Nikolai Varankine

#ifndef H_MatrixInput
#define H_MatrixInput

#include <stdint.h>
#include "esp32s2/rom/gpio.h"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class MatrixInput
{
public:
    /** Number of internal pad that holds constantly high value */
    static constexpr uint32_t SOURCE_HIGH = GPIO_FUNC_IN_HIGH;
    /** Number of internal pad that holds constantly low value */
    static constexpr uint32_t SOURCE_LOW  = GPIO_FUNC_IN_LOW;
public:
    /** GPIO pad number to get signal from, [0;53] or Constant::HIGH or Constant::LOW */
    SubValueRW* const source;
    /** 1: route signals via selected GPIO matrix pad; 
        0: connect signals directly to peripheral configured in IO_MUX */
    FlagRW* const matrix;
    /** Invert output of the channel */
    FlagRW* const invert;
    /** Index of the perpheral input channel */
    const size_t input;
public:
    /** @param i index of perpheral input, see "Table 22: GPIO Matrix" */
    MatrixInput( const size_t i );
    virtual ~MatrixInput();
};

#endif
