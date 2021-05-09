/* 
 * Single wire asynchronous data transfer in NZR communication mode; employs RMT output channel.
 * 
 * Every data bit in sequence is transferred by one of two predefined pulses of specific duty cycles, 
 * whether it is 0 or 1. Entire bit set is finished by pulse of third type, also of specific duty cycle.
 * 
 * Author © 2021 Nikolai Varankine
 */

#include <initializer_list>
#include <algorithm>
#include "SingleNZR.hpp"
#include "periphery/rmt/RemoteControlChannelMemory.hpp"

SingleNZR::SingleNZR( const RemoteControlController* const rmt, RemoteControlChannel* const channel, 
        ExternalPin* const pin, MatrixOutput* const output,
        const uint16_t n0H, const uint16_t n0L, 
        const uint16_t n1H, const uint16_t n1L, 
        const uint16_t nRST ) :
    rmt( rmt ), channel( channel ), pin( pin ), output( output ),
    rct( channel->getTransmitter() ),
    DATA
    {  
        // data 0
        RemoteControlChannelMemory::entry2( 
            RemoteControlChannelMemory::entry(  true, n0H ),
            RemoteControlChannelMemory::entry( false, n0L ) ),
        // data 1
        RemoteControlChannelMemory::entry2( 
            RemoteControlChannelMemory::entry(  true, n1H ),
            RemoteControlChannelMemory::entry( false, n1L ) ),
    },
    RESET( RemoteControlChannelMemory::entry2( 
            RemoteControlChannelMemory::entry( false, nRST ),
            RemoteControlChannelMemory::ENTRY_END ) ),
    thrLoop( 512*2 )
{
    channel->memory->setOwner( RemoteControlChannelMemory::Owner::SW_TRANSMITTER );
    channel->getClock()->setSource( RemoteControlClock::Source::APB );
    channel->getClock()->divider->set( 1 );

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
    
    
SingleNZR::~SingleNZR()
{
}

uint32_t SingleNZR::loadBytes( 
        const uint8_t* const values, uint32_t const size, const BitOrder order, 
        uint32_t const start, uint32_t const bound, const bool direct )
{
    uint32_t i = 0;
    for( ; i < size; i++ )
    {
        uint32_t address = start + i * 8;
        if( address + 8 > bound ) break; // no room for whole byte
        uint8_t value = values[i];
        for( int bit = 0; bit < 8; bit++ )
        {
            const uint8_t vi = value & static_cast<uint8_t>( order ) ? 1 : 0;
            loadEntries( DATA[ vi ], address + bit, bound, direct );
            if( order == BitOrder::MSBF )
                value <<= 1;
            else
                value >>= 1;
        }
    }
    return i;
}

void SingleNZR::loadEntries( const rmt_item32_t& data, 
        uint32_t const start, uint32_t const bound, const bool direct )
{
    if( direct )
    {
        RemoteControlRAM*  const ram  = channel->memory->ram;
        ram->setEntry2( start, data );
    }
    else
    {
        RemoteControlFIFO* const fifo = channel->memory->fifo;
        if( fifo->addressWrite->get() == bound ) //ATTN get() returns 0x1FF max
            fifo->reset->onOff(); // it re-enables memory I/O and transmitter's interrupt
        fifo->data->set( data.val );
    }
}

bool SingleNZR::waitFor( InterruptController *const interrupt )
{
    unsigned long pc = 0;
    while( ! interrupt->masked->get() && pc < thrLoop ) pc++;
    interrupt->clear->onOff();
    return pc < thrLoop;
}

int SingleNZR::transmit( const uint8_t* const values, uint32_t const size, const BitOrder order )
{
    RemoteControlFIFO* const fifo = channel->memory->fifo;
    RemoteControlRAM*  const ram  = channel->memory->ram;
    const uint32_t bound = 64 * ( channel->id + channel->memory->allocated->get() );
    const bool direct = rmt->memory->direct->get();

    // address reset is required
    ram->resetWrite->onOff();
    ram->resetRead->onOff();
    uint32_t start; //!< low bound of allocated memory
    if( direct )
    {
        // use direct RAM access
        start = ram->addressWrite->get();
    }
    else
    {
        // use FIFO gateway to memory
        fifo->reset->onOff(); 
        start = fifo->addressWrite->get();
    }
    
    const uint8_t* cursor = &values[0]; //!< beginning of shrinking source
    uint32_t remainder = size;

    // initial attempt to load full memory
    uint32_t loaded = loadBytes( cursor, remainder, order, start, bound, direct );
    cursor += loaded;
    remainder -= loaded;

    uint32_t const allocated = 64 * channel->memory->allocated->get();
    if( remainder > 0 || size * 8 == allocated )
    {
        // prepare to refill memory
        rct->limit->interrupt->enable->on();
        rct->limit->threshold->set( allocated / 2 );
    }
    else
    {
        rct->limit->interrupt->enable->off();
    }
    rct->limit->interrupt->clear->onOff();

    // monitor buffer read by transmitter
    channel->interrupt->enable->on();
    channel->interrupt->clear->onOff();

    // send in single or more passs if required
    rct->interrupt->enable->on();
    rct->interrupt->clear->onOff();
    ram->resetRead->onOff();
    //DO NOT channel->memory->fifo->reset->onOff();
    rct->send.on->on(); // start generation
    
    // refill data on request
    while( remainder > 0 )
    {
        // wait for refill request
        if( ! waitFor( rct->limit->interrupt ) ) 
            return abort( size - remainder, 0x04u );

        // move check point to next position
        uint32_t half = allocated / 2; // in 32 bit "pulse" words
        rct->limit->threshold->set( ( rct->limit->threshold->get() + half ) % ( rct->limit->threshold->max() + 1 ) );

        // refill memory
        half /= 8; // now in bytes of "values"
        uint32_t offset = ( ( size - remainder ) * 8  ) % allocated;
        uint32_t loaded = loadBytes( cursor, std::min( remainder, half ), order, start + offset, bound, direct );
        cursor += loaded;
        remainder -= loaded;

        // check the interrupt is needed any more
        rct->limit->interrupt->enable->set( loaded == half );
    }

    // append terminator
    if( rct->limit->interrupt->enable->get() ) 
    {
        // wait for refill request
        if( ! waitFor( rct->limit->interrupt ) ) 
            return abort( size - remainder, 0x02u );
        rct->limit->interrupt->enable->off();
    }
    // fill memory with terminator
    uint32_t offset = ( ( size - remainder ) * 8  ) % allocated;
    loadEntries( RESET, start + offset, bound, direct );

    // wait for end of generation    
    if( ! waitFor( rct->interrupt ) ) 
        return abort( size - remainder, 0x01u );
    rct->interrupt->enable->off();
    rct->interrupt->clear->onOff();
    rct->send.on->off();

    // done; final checks, cleanup and return
    if( channel->interrupt->masked->get() )
        return abort( size - remainder, 0x08u );
    channel->interrupt->enable->off();
    channel->interrupt->clear->onOff();
    rct->limit->interrupt->enable->off();
    rct->limit->interrupt->clear->onOff();
    return size - remainder;
}

int SingleNZR::abort( uint32_t const size, uint8_t const reason )
{
    rct->send.off->on(); // enforced stop of transmission
    rct->send.on->off(); // normal stop of transmission
    rct->send.off->off();
    //TODO send terminator or skip? rct is already configured to keep low when idle
    channel->memory->fifo->reset->onOff();
    channel->memory->ram->resetRead->onOff();
    channel->memory->ram->resetWrite->onOff();
    //TODO channel->status->get(); // 0x7 mask
    return 0x80000000u | ( (uint32_t) ( reason & 0x7F ) << 24 ) | std::min( size, 0x00FFFFFFu );
}
