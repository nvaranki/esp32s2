#include "soc/gpio_reg.h"
#include "MatrixBuffer.hpp"

MatrixBuffer::MatrixBuffer() :
    inp0( new WordRO( GPIO_IN_REG ) ),
    inp1( new SubValueRO( GPIO_IN1_REG, GPIO_IN1_DATA_M, GPIO_IN1_DATA_S ) ),
    out0( new WordWO( GPIO_OUT_REG ) ),
    out1( new SubValueWO( GPIO_OUT1_REG, GPIO_OUT1_DATA_M, GPIO_OUT1_DATA_S ) ),
    set0( new WordWO( GPIO_OUT_W1TS_REG ) ),
    set1( new SubValueWO( GPIO_OUT1_W1TS_REG, GPIO_OUT1_W1TS_M, GPIO_OUT1_W1TS_S ) ),
    clr0( new WordWO( GPIO_OUT_W1TC_REG ) ),
    clr1( new SubValueWO( GPIO_OUT1_W1TC_REG, GPIO_OUT1_W1TC_M, GPIO_OUT1_W1TC_S ) ),
    enw0( new WordRW( GPIO_ENABLE_REG ) ),
    enw1( new SubValueRW( GPIO_ENABLE1_REG, GPIO_ENABLE1_DATA_M, GPIO_ENABLE1_DATA_S ) ),
    ens0( new WordWO( GPIO_ENABLE_W1TS_REG ) ),
    ens1( new SubValueWO( GPIO_ENABLE1_W1TS_REG, GPIO_ENABLE1_W1TS_M, GPIO_ENABLE1_W1TS_S ) ),
    enc0( new WordWO( GPIO_ENABLE_W1TC_REG ) ),
    enc1( new SubValueWO( GPIO_ENABLE1_W1TC_REG, GPIO_ENABLE1_W1TC_M, GPIO_ENABLE1_W1TC_S ) )
{
}

MatrixBuffer::~MatrixBuffer()
{
    delete inp0;
    delete inp1;
    delete out0;
    delete out1;
    delete set0;
    delete set1;
    delete clr0;
    delete clr1;
    delete enw0;
    delete enw1;
    delete ens0;
    delete ens1;
    delete enc0;
    delete enc1;
}

uint64_t MatrixBuffer::read() const
{
    return ( (uint64_t) inp1->get() << 32 ) | inp0->get();
}

void MatrixBuffer::write( uint64_t v )
{
    out0->set( (uint32_t) ( v & 0xFFFFFFFFuL ) );
    out1->set( (uint32_t) ( ( v >> 32 ) & 0x003FFFFFuL ) );
}

void MatrixBuffer::set( uint64_t m )
{
    set0->set( (uint32_t) ( m & 0xFFFFFFFFuL ) );
    set1->set( (uint32_t) ( ( m >> 32 ) & 0x003FFFFFuL ) );
}

void MatrixBuffer::clear( uint64_t m )
{
    clr0->set( (uint32_t) ( m & 0xFFFFFFFFuL ) );
    clr1->set( (uint32_t) ( ( m >> 32 ) & 0x003FFFFFuL ) );
}

uint64_t MatrixBuffer::enabled() const
{
    return ( (uint64_t) enw1->get() << 32 ) | enw0->get();
}

void MatrixBuffer::enable( uint64_t v )
{
    enw0->set( (uint32_t) ( v & 0xFFFFFFFFuL ) );
    enw1->set( (uint32_t) ( ( v >> 32 ) & 0x003FFFFFuL ) );
}

void MatrixBuffer::setEnabled( uint64_t m )
{
    ens0->set( (uint32_t) ( m & 0xFFFFFFFFuL ) );
    ens1->set( (uint32_t) ( ( m >> 32 ) & 0x003FFFFFuL ) );
}

void MatrixBuffer::clearEnabled( uint64_t m )
{
    enc0->set( (uint32_t) ( m & 0xFFFFFFFFuL ) );
    enc1->set( (uint32_t) ( ( m >> 32 ) & 0x003FFFFFuL ) );
}
