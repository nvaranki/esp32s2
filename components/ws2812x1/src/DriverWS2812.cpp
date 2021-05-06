/* 
 * Single/pair WS2812 LED driver. Employs RMT output channel.
 *
 * Author © 2021 Nikolai Varankine
 */

#include <stdio.h>
#include <initializer_list>
#include "DriverWS2812.hpp"
#include "periphery/rmt/RemoteControlChannelMemory.hpp"

constexpr uint16_t T0H = 350u; // 0 code, high voltage time, ns
constexpr uint16_t T0L = 800u; // 0 code,  low voltage time, ns
constexpr uint16_t T1H = 700u; // 1 code, high voltage time, ns
constexpr uint16_t T1L = 600u; // 1 code,  low voltage time, ns
constexpr uint16_t RES = 50000u; // low voltage time, ns

DriverWS2812::DriverWS2812( MicroControllerUnit* const mcu, const uint8_t ch, const uint8_t gpio ) :
    io( mcu->getControllerIO() ),
    rmt( mcu->periphery.getRemoteControlController() ),
    rcc( rmt->getChannel( ch ) ),
    pin( io->getExternalPin( gpio ) ),
    output( io->getMatrixOutput( pin->number ) ),
    DATA
    {  
        // APB frequency 80 MHz -> 12.5 ns == 25/2 ns
        // data 0
        RemoteControlChannelMemory::entry2( 
            RemoteControlChannelMemory::entry(  true, T0H / 25 * 2 ),
            RemoteControlChannelMemory::entry( false, T0L / 25 * 2 ) ),
        // data 1
        RemoteControlChannelMemory::entry2( 
            RemoteControlChannelMemory::entry(  true, T1H / 25 * 2 ),
            RemoteControlChannelMemory::entry( false, T1L / 25 * 2 ) ),
    },
    RESET( RemoteControlChannelMemory::entry2( 
            RemoteControlChannelMemory::entry( false, RES / 25 * 2 + 1 ), // more than min
            RemoteControlChannelMemory::ENTRY_END ) )
{
    rmt->reset->off();
    rmt->clock->on();
    rmt->enable->on();
    rmt->memory->wrap->off();
    rmt->memory->direct->on();

    rcc->memory->setOwner( RemoteControlChannelMemory::Owner::SW_TRANSMITTER );
    rcc->getClock()->setSource( RemoteControlClock::Source::APB );
    rcc->getClock()->divider->set( 1 );

    RemoteControlTransmitter* rct = rcc->getTransmitter();
    rct->send.on->off();
    rct->send.off->off();
    rct->idle->level->low();
    rct->idle->output->on();
    rct->interrupt->enable->off();
    rct->limit->interrupt->enable->off();
    rct->carrier->enable->off();

    pin->input->off();
    pin->function->set( 1 ); //TODO mnemonic
    pin->sleep.enable->off();

    output->periphery->set( rct->index );
    output->enable.setSource( MatrixOutput::Enable::Source::PERIPHERY );
}

DriverWS2812::~DriverWS2812()
{
}

void DriverWS2812::push()
{
    RemoteControlTransmitter* const rct = rcc->getTransmitter();
    
    // send in single pass, address reset required
    rct->interrupt->enable->on();
    rct->interrupt->clear->on();
    rct->interrupt->clear->off();
    rcc->memory->fifo->reset->on();
    rcc->memory->fifo->reset->off();
    rcc->memory->ram->resetRead->on();
    rcc->memory->ram->resetRead->off();
    rct->send.on->on();
    // generating - counting ...
    unsigned long pc = 0;
    while( ! rct->interrupt->masked->get() && pc < 1000u ) { pc++; } // pc~330 per RGB triple; ~ 30 + 50 mks
    rct->interrupt->enable->off();
    rct->interrupt->clear->on();
    rct->interrupt->clear->off();
    rct->send.on->off();
}

void DriverWS2812::load( uint8_t* const values, uint32_t const size )
{
    RemoteControlTransmitter* const rct = rcc->getTransmitter();
    
    // using FIFO memory load, address reset required
    rcc->memory->fifo->reset->on();
    rcc->memory->fifo->reset->off();
    rcc->memory->ram->resetRead->on();
    rcc->memory->ram->resetRead->off();
    WordRW* const fifo = rcc->memory->fifo->data;
    for( int i = 0; i < size; i++ )
    {
        uint8_t v = values[i];
        for( int bit = 0; bit < 8; bit++ )
        {
            fifo->set( DATA[ v & 0x80u ? 1 : 0 ].val ); 
            v <<= 1;
        }
    }
    fifo->set( DriverWS2812::RESET.val ); 
}
    
void DriverWS2812::send( const uint8_t r, const uint8_t g, const uint8_t b )
{
    bool direct = rmt->memory->direct->get();
    rmt->memory->direct->off();
    uint8_t values[] { g, r, b };
    load( values, sizeof( values ) / sizeof( uint8_t ) );
    push();
    rmt->memory->direct->set( direct );
}

void DriverWS2812::send( 
        const uint8_t r0, const uint8_t g0, const uint8_t b0, 
        const uint8_t r1, const uint8_t g1, const uint8_t b1 )
{
    bool direct = rmt->memory->direct->get();
    rmt->memory->direct->off();
    uint8_t values[] { g0, r0, b0, g1, r1, b1 };
    load( values, sizeof( values ) / sizeof( uint8_t ) );
    push();
    rmt->memory->direct->set( direct );
}
