// Interrupt controller of GPIO Matrix.
//
// © 2021 Nikolai Varankine

#include "soc/gpio_reg.h"
#include "MatrixInterrupt.hpp"

MatrixInterrupt::MatrixInterrupt() :
    src0( new WordRO( GPIO_STATUS_NEXT_REG ) ),
    src1( new SubValueRO( GPIO_STATUS_NEXT1_REG, GPIO_STATUS_INTERRUPT_NEXT1_M, GPIO_STATUS_INTERRUPT_NEXT1_S ) ),
    stw0( new WordRW( GPIO_STATUS_REG ) ),
    stw1( new SubValueRW( GPIO_STATUS1_REG, GPIO_STATUS1_INT_M, GPIO_STATUS1_INT_S ) ),
    sts0( new WordWO( GPIO_STATUS_W1TS_REG ) ),
    sts1( new SubValueWO( GPIO_STATUS1_W1TS_REG, GPIO_STATUS1_W1TS_M, GPIO_STATUS1_W1TS_S ) ),
    stc0( new WordWO( GPIO_STATUS_W1TC_REG ) ),
    stc1( new SubValueWO( GPIO_STATUS1_W1TC_REG, GPIO_STATUS1_W1TC_M, GPIO_STATUS1_W1TC_S ) )
{
}

MatrixInterrupt::~MatrixInterrupt()
{
    delete src0;
    delete src1;
    delete stw0;
    delete stw1;
    delete sts0;
    delete sts1;
    delete stc0;
    delete stc1;
}

uint64_t MatrixInterrupt::source() const
{
    return ( (uint64_t) src1->get() << 32 ) | src0->get();
}

uint64_t MatrixInterrupt::status() const
{
    return ( (uint64_t) stw1->get() << 32 ) | stw0->get();
}

void MatrixInterrupt::write( uint64_t v )
{
    stw0->set( (uint32_t) ( v & 0xFFFFFFFFuL ) );
    stw1->set( (uint32_t) ( ( v >> 32 ) & 0x003FFFFFuL ) );
}

void MatrixInterrupt::set( uint64_t m )
{
    sts0->set( (uint32_t) ( m & 0xFFFFFFFFuL ) );
    sts1->set( (uint32_t) ( ( m >> 32 ) & 0x003FFFFFuL ) );
}

void MatrixInterrupt::clear( uint64_t m )
{
    stc0->set( (uint32_t) ( m & 0xFFFFFFFFuL ) );
    stc1->set( (uint32_t) ( ( m >> 32 ) & 0x003FFFFFuL ) );
}
