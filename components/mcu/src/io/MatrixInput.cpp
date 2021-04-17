// Input multiplexor of GPIO Matrix.
//
// © 2021 Nikolai Varankine

#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "MatrixInput.hpp"

MatrixInput::MatrixInput( const size_t n ) :
    source( new SubValueRW( GPIO_FUNC0_IN_SEL_CFG_REG + n * 0x4, GPIO_FUNC0_IN_SEL_M, GPIO_FUNC0_IN_SEL_S ) ),
    matrix( new FlagRW(  GPIO_FUNC0_IN_SEL_CFG_REG + n * 0x4, GPIO_SIG0_IN_SEL_S ) ),
    invert( new FlagRW(  GPIO_FUNC0_IN_SEL_CFG_REG + n * 0x4, GPIO_FUNC0_IN_INV_SEL_S ) ),
    input( n )
{
}
    
MatrixInput::~MatrixInput()
{
    delete source;
    delete matrix;
    delete invert;
}
