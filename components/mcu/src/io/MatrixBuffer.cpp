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
    for( int i = 0; i < MAX_CHANNEL; i++ ) channel[i] = nullptr;
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
    for( int i = 0; i < MAX_CHANNEL; i++ ) if( channel[i] != nullptr ) delete channel[i];
}

uint64_t MatrixBuffer::read() const
{
    return ( (uint64_t) inp1->get() << 32 ) | inp0->get();
}

void MatrixBuffer::write( uint64_t v )
{
    out0->set( (uint32_t) ( v & MASK_L ) );
    out1->set( (uint32_t) ( ( v >> 32 ) & MASK_H ) );
}

void MatrixBuffer::set( uint64_t m )
{
    set0->set( (uint32_t) ( m & MASK_L ) );
    set1->set( (uint32_t) ( ( m >> 32 ) & MASK_H ) );
}

void MatrixBuffer::clear( uint64_t m )
{
    clr0->set( (uint32_t) ( m & MASK_L ) );
    clr1->set( (uint32_t) ( ( m >> 32 ) & MASK_H ) );
}

uint64_t MatrixBuffer::enabled() const
{
    return ( (uint64_t) enw1->get() << 32 ) | enw0->get();
}

void MatrixBuffer::enable( uint64_t v )
{
    enw0->set( (uint32_t) ( v & MASK_L ) );
    enw1->set( (uint32_t) ( ( v >> 32 ) & MASK_H ) );
}

void MatrixBuffer::setEnabled( uint64_t m )
{
    ens0->set( (uint32_t) ( m & MASK_L ) );
    ens1->set( (uint32_t) ( ( m >> 32 ) & MASK_H ) );
}

void MatrixBuffer::clearEnabled( uint64_t m )
{
    enc0->set( (uint32_t) ( m & MASK_L ) );
    enc1->set( (uint32_t) ( ( m >> 32 ) & MASK_H ) );
}

MatrixBuffer::Channel* MatrixBuffer::getChannel( const size_t i )
{
    if( i >= MAX_CHANNEL ) return nullptr;
    if( channel[i] == nullptr ) channel[i] = new Channel( this, i );
    return channel[i];
}

void MatrixBuffer::Channel::enable( const bool v )
{
    if( v )
    {
        if( id < 32 )
            mb->ens0->set( mask );
        else
            mb->ens1->set( mask );
    }
    else
    {
        if( id < 32 )
            mb->enc0->set( mask );
        else
            mb->enc1->set( mask );
    }
}

void MatrixBuffer::Channel::write( const bool v )
{
    if( v )
    {
        if( id < 32 )
            mb->set0->set( mask );
        else
            mb->set1->set( mask );
    }
    else
    {
        if( id < 32 )
            mb->clr0->set( mask );
        else
            mb->clr1->set( mask );
    }
}

bool MatrixBuffer::Channel::read() const
{
    return ( id < 32 ? mb->inp0->get() : mb->inp1->get() ) & mask;
}
