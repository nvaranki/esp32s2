#include "soc/gpio_reg.h"
#include "MatrixBuffer.hpp"
#include "MatrixOutput.hpp"

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

uint64_t MatrixBuffer::getOutput() const
{
    return ( (uint64_t) enw1->get() << 32 ) | enw0->get();
}

void MatrixBuffer::setOutput( uint64_t v )
{
    enw0->set( (uint32_t) ( v & MASK_L ) );
    enw1->set( (uint32_t) ( ( v >> 32 ) & MASK_H ) );
}

void MatrixBuffer::enableOutput( uint64_t m )
{
    ens0->set( (uint32_t) ( m & MASK_L ) );
    ens1->set( (uint32_t) ( ( m >> 32 ) & MASK_H ) );
}

void MatrixBuffer::disableOutput( uint64_t m )
{
    enc0->set( (uint32_t) ( m & MASK_L ) );
    enc1->set( (uint32_t) ( ( m >> 32 ) & MASK_H ) );
}

MatrixBuffer::Channel::Channel( MatrixBuffer* mb, const size_t id ) : 
    mb( mb ), mask( BIT( id % 32 ) ), 
    periphery( new SubValueRW( GPIO_FUNC0_OUT_SEL_CFG_REG + id * 0x4, GPIO_FUNC0_OUT_SEL_M, GPIO_FUNC0_OUT_SEL_S ) ),
    id( id ) 
{
}
    
MatrixBuffer::Channel::~Channel() 
{
    delete periphery;
}

MatrixBuffer::Channel* MatrixBuffer::getChannel( const size_t i )
{
    if( i >= MAX_CHANNEL ) return nullptr;
    if( channel[i] == nullptr ) channel[i] = new Channel( this, i );
    return channel[i];
}

bool MatrixBuffer::Channel::output() const
{
    return ( id < 32 ? mb->enw0->get() : mb->enw1->get() ) & mask;
}

void MatrixBuffer::Channel::output( const bool v )
{
    if( id < 32 )
        ( v ? mb->ens0 : mb->enc0 )->set( mask );
    else
        ( v ? mb->ens1 : mb->enc1 )->set( mask );
}

void MatrixBuffer::Channel::write( const bool v )
{
    if( id < 32 )
        ( v ? mb->set0 : mb->clr0 )->set( mask );
    else
        ( v ? mb->set1 : mb->clr1 )->set( mask );
}

bool MatrixBuffer::Channel::read() const
{
    return ( id < 32 ? mb->inp0->get() : mb->inp1->get() ) & mask;
}

void MatrixBuffer::Channel::regain()
{
    periphery->set( MatrixOutput::PERIPHERY_BUFFER );
}
