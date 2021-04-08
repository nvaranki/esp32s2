// Output multiplexor of GPIO Matrix.
//
// © 2021 Nikolai Varankine

#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "MatrixOutput.hpp"

MatrixOutput::MatrixOutput( const size_t n ) :
    source( new SubValueRW( GPIO_FUNC0_OUT_SEL_CFG_REG + n * 0x4, GPIO_FUNC0_OUT_SEL_M, GPIO_FUNC0_OUT_SEL_S ) ),
    enable( n ),
    invert( new FlagRW(     GPIO_FUNC0_OUT_SEL_CFG_REG + n * 0x4, GPIO_FUNC0_OUT_INV_SEL_S ) )
{
}
    
MatrixOutput::~MatrixOutput()
{
    delete source;
    delete invert;
}

MatrixOutput::Enable::Enable( const size_t n ) :
    select( new FlagRW( GPIO_FUNC0_OUT_SEL_CFG_REG + n * 0x4, GPIO_FUNC0_OEN_SEL_S ) ),
    invert( new FlagRW( GPIO_FUNC0_OUT_SEL_CFG_REG + n * 0x4, GPIO_FUNC0_OEN_INV_SEL_S ) )
{
}
    
MatrixOutput::Enable::~Enable()
{
    delete select;
    delete invert;
}
