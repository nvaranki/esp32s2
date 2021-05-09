/* GPIO Example: Ouput signal from peripheral unit to GPIO pin.

   The program uses hardware digital generator RMT to make digital signal.
   That, in turn, is received by pulse counter, as well as RMT receiver.
   GPIO Matrix is exploited.

   !!! PIN_OUT must connect PIN_INP by wire to allow proper function.

   See expected output in "output.txt".

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include <initializer_list>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "mcu.h"

#define PIN_OUT 4
#define PIN_INP 8
#define FUN_OUT FUNC_GPIO4_GPIO4 //TODO FUNC_GPIO4_GPIO4_0 ?
#define FUN_INP FUNC_GPIO8_GPIO8 //TODO FUNC_GPIO8_GPIO8_0 ?

void printf( const PulseCountUnit* pcu, const char* comment )
{
    printf( "Unit %d: status=0x%02x counter=%+d %s\n", 
        pcu->id, (uint32_t) pcu->counter.getStatus(), pcu->counter.getValue(), comment );
}

void printf( const MatrixInput* mi, const ExternalPin* p )
{
    printf( "Peripheral input %d assigned to GPIO pin %02d %1s %s\n", 
        mi->periphery, mi->gpio->get(), p == nullptr ? "F" : p->input->get() ? "R" : "W",
        mi->matrix->get() ? "via matrix" : "directly" );
}

void printf( const MatrixOutput* mo, const ExternalPin* p )
{
    printf( "Peripheral output %d assigned to GPIO pin %02d %1s\n", 
        mo->periphery->get(), mo->gpio, p == nullptr ? "F" : p->input->get() ? "R" : "W" );
}

void printf( const InterruptController* ic, const char* comment = "" )
{
    printf( "Interrupt: E=%1d R=%1d M=%1d %s\n", 
        ic->enable->get(),
        ic->raw->get(),
        ic->masked->get(),
        comment );
}

void ram( RemoteControlController* const rmt, RemoteControlChannel* const rcc0, PulseCountUnit* const pcu,
    const rmt_item16_t& p, const rmt_item16_t& n )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    RemoteControlRAM*         const ram = rcc0->memory->ram;
    PulseCountUnit::Counter*  const cnt = &pcu->counter;

    printf( "\nDirect memory load, single pass, address reset\n" );
    rmt->memory->direct->on();
    rmt_item16_t e[7]{ p, n, p, n, p, n, RemoteControlChannelMemory::ENTRY_END };
    printf( "TX:" );
    for( int i = 0; i < sizeof(e)/sizeof(rmt_item16_t); i++ )
    {
        ram->setEntry( i, e[i] ); 
        printf( " %d=%04hx", i, ram->getEntry( i ).val );
    }
    printf( "\n" );
    cnt->reset->on();
    cnt->reset->off();
    ram->resetRead->on();
    ram->resetRead->off();
    printf( "Address  RAM: R=%d W=%d\n", ram->addressRead->get(), ram->addressWrite->get() ); 
    rct->send.on->on(); // required for both memory and FIFO
    // generating - counting ...
    while( ! rct->interrupt->raw->get() ) {}
    rct->interrupt->clear->on();
    rct->send.on->off();
    printf( "Address  RAM: R=%d W=%d\n", ram->addressRead->get(), ram->addressWrite->get() ); //TODO R=5 W=0
    ram->resetRead->on();
    ram->resetRead->off();
    printf( "Address  RAM: R=%d W=%d\n", ram->addressRead->get(), ram->addressWrite->get() ); 
    printf( pcu, "" ); // 3
}

void fifo( RemoteControlController* const rmt, RemoteControlChannel* const rcc0, PulseCountUnit* const pcu,
    const rmt_item16_t& p, const rmt_item16_t& n )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    RemoteControlRAM*         const ram = rcc0->memory->ram;
    RemoteControlFIFO*       const fifo = rcc0->memory->fifo;
    PulseCountUnit::Counter*  const cnt = &pcu->counter;

    printf( "\nFIFO memory load, single pass, address reset\n" );
    rmt->memory->direct->off();
    fifo->reset->on();
    fifo->reset->off();
    printf( "Address FIFO: R=%d W=%d\n", fifo->addressRead->get(), fifo->addressWrite->get() ); 
    rmt_item16_t e1[6]{ p, n, p, n, RemoteControlChannelMemory::ENTRY_END, n };
    for( int i = 0; i < sizeof(e1)/sizeof(rmt_item16_t); i +=2 )
        fifo->data->set( RemoteControlChannelMemory::entry2( e1[i], e1[i+1] ).val );
    // fires rcc0->interrupt if "i" goes beyond allocated memory
    printf( "Address FIFO: R=%d W=%d\n", fifo->addressRead->get(), fifo->addressWrite->get() ); 
    printf( "Address  RAM: R=%d W=%d\n", ram->addressRead->get(), ram->addressWrite->get() ); 
    cnt->reset->on();
    cnt->reset->off();
    ram->resetRead->on(); // send goes from RAM, do not reset FIFO
    ram->resetRead->off();
    rct->send.on->on(); // required for both memory and FIFO
    // generating - counting ...
    while( ! rct->interrupt->raw->get() ) {}
    rct->interrupt->clear->on();
    rct->send.on->off();
    printf( "Address FIFO: R=%d W=%d\n", fifo->addressRead->get(), fifo->addressWrite->get() ); 
    printf( "Address  RAM: R=%d W=%d\n", ram->addressRead->get(), ram->addressWrite->get() ); 
    fifo->reset->on();
    fifo->reset->off();
    ram->resetRead->on();
    ram->resetRead->off();
    printf( "Address FIFO: R=%d W=%d\n", fifo->addressRead->get(), fifo->addressWrite->get() ); 
    printf( "Address  RAM: R=%d W=%d\n", ram->addressRead->get(), ram->addressWrite->get() ); 
    printf( "TX:" );
    for( int i = 0; i < sizeof(e1)/sizeof(rmt_item16_t); i +=2 )
    {
        uint32_t val = fifo->data->get();
        printf( " %d=%04hx", i,   val & 0x0000FFFF );
        printf( " %d=%04hx", i+1, (( val & 0xFFFF0000 ) >> 16 ) & 0x0000FFFF );
    }
    printf( " - FIFO\n" );
    printf( "Address FIFO: R=%d W=%d\n", fifo->addressRead->get(), fifo->addressWrite->get() ); 
    printf( "Address  RAM: R=%d W=%d\n", ram->addressRead->get(), ram->addressWrite->get() ); 
    rmt->memory->direct->on(); // reads trash if not set
    printf( "TX:" );
    for( int i = 0; i < sizeof(e1)/sizeof(rmt_item16_t); i +=2 )
    {
        printf( " %d=%04hx", i,   ram->getEntry( i   ).val );
        printf( " %d=%04hx", i+1, ram->getEntry( i+1 ).val );
    }
    printf( " - RAM\n" );
    printf( pcu, "" ); // 2
}

void continuous( RemoteControlChannel* const rcc0, PulseCountUnit* const pcu, 
    const rmt_item16_t& p, const rmt_item16_t& n )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    PulseCountUnit::Counter*  const cnt = &pcu->counter;
    
    printf( "\nPasses in unlimited continuous mode\n" );
    rmt_item16_t e[3]{ p, n, RemoteControlChannelMemory::ENTRY_END };
    printf( "TX:" );
    for( int i = 0; i < sizeof(e)/sizeof(rmt_item16_t); i++ )
    {
        rcc0->memory->ram->setEntry( i, e[i] ); 
        printf( " %d=%04hx", i, rcc0->memory->ram->getEntry( i ).val );
    }
    printf( "\n" );
    cnt->reset->on();
    cnt->reset->off();
    rct->continuous->on(); // repeats memory contents for this channel
    rcc0->memory->ram->resetRead->on();
    rcc0->memory->ram->resetRead->off();
    rct->send.on->on(); // required for both memory and FIFO
    // generating - counting ...
    vTaskDelay( 1 );
    rct->send.off->on(); // forced stop
    rct->send.on->off();
    rct->send.off->off();
    rct->continuous->off(); // it doesn't stop alone
    printf( pcu, "" ); // ~3910 vTaskDelay( 1 )
}

void loop( RemoteControlChannel* const rcc0, PulseCountUnit* const pcu, 
    const rmt_item16_t& p, const rmt_item16_t& n, uint32_t const repeat )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    PulseCountUnit::Counter*  const cnt = &pcu->counter;

    printf( "\nPasses in counted loop, monitored interrupt\n" );
    rmt_item16_t e[3]{ p, n, RemoteControlChannelMemory::ENTRY_END };
    printf( "TX:" );
    for( int i = 0; i < sizeof(e)/sizeof(rmt_item16_t); i++ )
    {
        rcc0->memory->ram->setEntry( i, e[i] ); 
        printf( " %d=%04hx", i, rcc0->memory->ram->getEntry( i ).val );
    }
    printf( "\n" );
    rct->loop->reset->on(); // to start counting from 0
    rct->loop->reset->off();
    rct->loop->interrupt->enable->on();
    rct->loop->interrupt->clear->on();
    rct->loop->repeat->set( repeat ); // it looks like counting from zero
    rct->loop->enable->on(); // enable counting of repeats
    printf( "TX LOOP R=%d\n", rct->loop->repeat->get() ); 
    printf( rct->loop->interrupt, "LOOP type" ); 
    cnt->reset->on();
    cnt->reset->off();
    rct->continuous->on(); // repeats memory contents
    rcc0->memory->ram->resetRead->on();
    rcc0->memory->ram->resetRead->off();
    rct->send.on->on(); // required for both memory and FIFO
    // generating - counting ...
    unsigned long pc = 0;
    while( ! rct->loop->interrupt->masked->get() ) { pc++; } // ~15 checks per entry couple
    rct->send.off->on(); // stop immediately
    rct->send.on->off();
    rct->send.off->off();
    printf( "Check count: %ld\n", pc ); 
    printf( rct->loop->interrupt, "fired" ); 
    rct->loop->interrupt->clear->on();
    printf( rct->loop->interrupt, "cleared" ); 
    rct->continuous->off();
    rct->loop->enable->off();
    rct->loop->interrupt->enable->off();
    printf( pcu, "" ); // 101 for repeat=100
}

void wrap( RemoteControlController* const rmt, RemoteControlChannel* const rcc0, 
    PulseCountUnit* const pcu, const rmt_item16_t& p, const rmt_item16_t& n )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    PulseCountUnit::Counter*  const cnt = &pcu->counter;

    printf( "\nPasses in limited wrap mode, interrupts\n" );
    for( int i = 0; i < 128; i++ ) // full block
    {
        rcc0->memory->ram->setEntry( i, ( i & 0x1 ) ? n : p ); 
        printf( " %3d=%04hx", i, rcc0->memory->ram->getEntry( i ).val );
        if( (i+1) % 16 == 0 ) printf( "\n" );
    }
    rct->interrupt->enable->on();
    rct->interrupt->clear->on();
    printf( rct->interrupt, "TX type" ); 
    rct->limit->threshold->set( 224 ); // 64*3.5 turns around
    printf( "TX THR @ %d\n", rct->limit->threshold->get() ); 
    rct->limit->interrupt->enable->on();
    rct->limit->interrupt->clear->on(); // apply to enabled
    printf( rct->limit->interrupt, "THR type" ); 
    cnt->reset->on();
    cnt->reset->off();
    rmt->memory->wrap->on();
    rcc0->memory->ram->resetRead->on();
    rcc0->memory->ram->resetRead->off();
    rct->send.on->on(); // required for both memory and FIFO
    // generating - counting ...
    unsigned long pcT = 0;
    while( ! rct->limit->interrupt->masked->get() ) { pcT++; } // ~15 checks per entry couple
    rcc0->memory->ram->setEntry( 20, RemoteControlChannelMemory::ENTRY_END ); // to finish on the next cycle at entry #(4*128+20)
    printf( "Address  RAM: R=%d W=%d\n", rcc0->memory->ram->addressRead->get(), rcc0->memory->ram->addressWrite->get() ); 
    printf( "Check count: %ld\n", pcT ); 
    printf( rct->limit->interrupt, "THR fired" ); 
    rct->limit->interrupt->clear->on();
    printf( rct->limit->interrupt, "THR cleared" ); 
    pcT = 0;
    while( ! rct->interrupt->masked->get() ) { pcT++; } // no cycles cause slow printf's above
    printf( rct->interrupt, "TX fired" ); 
    printf( "Check count: %ld\n", pcT ); 
    rct->interrupt->clear->on();
    printf( rct->interrupt, "TX cleared" ); 
    rct->send.on->off(); // already stopped, no rush
    rmt->memory->wrap->off();
    rct->limit->interrupt->enable->off();
    rct->interrupt->enable->off();
    printf( pcu, "" ); // 4*64+20/2=266
}

void frequency( RemoteControlChannel* const rcc0, PulseCountUnit* const pcu, uint16_t const d )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    PulseCountUnit::Counter*  const cnt = &pcu->counter;

    printf( "\nPasses at max frequency %d kHz\n", 80000/d/2 ); // APB at 80 MHz
    // d: 1=40MHz (no osc confirm) 2=20MHz 5=8MHz 10=4MHz ...
    rcc0->memory->ram->setEntry( 0, RemoteControlChannelMemory::entry(  true, d ) ); 
    rcc0->memory->ram->setEntry( 1, RemoteControlChannelMemory::entry( false, d ) ); 
    rcc0->memory->ram->setEntry( 2, RemoteControlChannelMemory::entry(  true, d ) ); 
    rcc0->memory->ram->setEntry( 3, RemoteControlChannelMemory::entry( false, d ) ); 
    rcc0->memory->ram->setEntry( 4, RemoteControlChannelMemory::entry(  true, d ) ); 
    rcc0->memory->ram->setEntry( 5, RemoteControlChannelMemory::entry( false, d ) ); 
    rcc0->memory->ram->setEntry( 6, RemoteControlChannelMemory::entry(  true, d ) ); 
    rcc0->memory->ram->setEntry( 7, RemoteControlChannelMemory::entry( false, d ) ); 
    rcc0->memory->ram->setEntry( 8, RemoteControlChannelMemory::entry(  true, d ) ); 
    rcc0->memory->ram->setEntry( 9, RemoteControlChannelMemory::entry( false, d ) ); 
    rcc0->memory->ram->setEntry(10, RemoteControlChannelMemory::ENTRY_END         ); 
    rct->interrupt->enable->on();
    rct->interrupt->clear->on();
    printf( rct->interrupt, "TX type" ); 
    cnt->reset->on();
    cnt->reset->off();
    rcc0->memory->ram->resetRead->on();
    rcc0->memory->ram->resetRead->off();
    rct->send.on->on(); // required for both memory and FIFO
    // generating - counting ...
    unsigned long pcT = 0;
    while( ! rct->interrupt->masked->get() ) { pcT++; } // no cycles cause slow printf's above
    rct->send.on->off(); // already stopped, no rush
    printf( rct->interrupt, "TX fired" ); 
    printf( "Check count: %ld\n", pcT ); 
    rct->interrupt->clear->on();
    printf( rct->interrupt, "TX cleared" ); 
    printf( pcu, "" ); // 5@40 6@20 5@8 5@4MHz
}

void modulator( RemoteControlChannel* const rcc0, PulseCountUnit* const pcu, 
    const bool m, const rmt_item16_t* const e, const uint32_t es )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    PulseCountUnit::Counter*  const cnt = &pcu->counter;

    printf( "\nGeneration of %s signal\n", m ? "modulated" : "unmodulated" );
    printf( "TX:" );
    for( int i = 0; i < es; i++ )
    {
        rcc0->memory->ram->setEntry( i, e[i] ); 
        printf( " %d=%04hx", i, rcc0->memory->ram->getEntry( i ).val );
        if( (i+1) % 16 == 0 || i == es - 1 ) printf( "\n" );
    }
    rcc0->level->on(); // modulate 1's
    rcc0->getClock()->divider->set( 16 ); // 80/16=5 MHz 200 ns 
    rct->carrier->limited->on(); // data only
    rct->carrier->high->set( 32 ); // 12.5*32=400 ns
    rct->carrier->low->set( 96 ); // 12.5*96=1200 ns
    rct->carrier->enable->set( m );
    printf( "Carrier TX: M=%1d V=%1d L=%d H=%d\n", 
        rct->carrier->enable->get(), rcc0->level->get(),
        rct->carrier->low->get(), rct->carrier->high->get() );
    // rcc0->getReceiver()->carrier->enable->on();//TODO DEBUG
    rct->interrupt->enable->on();
    rct->interrupt->clear->on();
    printf( rct->interrupt, "TX type" ); 
    cnt->reset->on();
    cnt->reset->off();
    rcc0->memory->ram->resetRead->on();
    rcc0->memory->ram->resetRead->off();
    printf( "Address  RAM: R=%d W=%d\n", rcc0->memory->ram->addressRead->get(), rcc0->memory->ram->addressWrite->get() ); 
    rct->send.on->on();
    // generating - counting ...
    unsigned long pc = 0;
    while( ! rct->interrupt->masked->get() ) { pc++; } // no cycles cause slow printf's above
    rct->send.on->off(); // already stopped, no rush
    printf( "Check count TX: %ld\n", pc ); 
    printf( rct->interrupt, "TX fired" ); 
    rct->interrupt->clear->on();
    printf( rct->interrupt, "TX cleared" ); 
    printf( "Address  RAM: R=%d W=%d\n", rcc0->memory->ram->addressRead->get(), rcc0->memory->ram->addressWrite->get() ); 
    printf( pcu, "" ); // 6
    printf( rct->interrupt, "RX cleared" );
    //TODO corrupts repeat() rcc0->getClock()->divider->set( 1 );
}

void recognition( RemoteControlController* const rmt, RemoteControlChannel* const rcc0, 
    RemoteControlChannel* const rcc1, PulseCountUnit* const pcu, 
    const bool m, const rmt_item16_t* const e, const uint32_t es )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    RemoteControlReceiver*    const rcr = rcc1->getReceiver();
    PulseCountUnit::Counter*  const cnt = &pcu->counter;

    printf( "\nRecognition of %s signal\n", m ? "modulated" : "unmodulated" );
    printf( "TX:" );
    for( int i = 0; i < es; i++ )
    {
        rcc0->memory->ram->setEntry( i, e[i] ); 
        printf( " %d=%04hx", i, rcc0->memory->ram->getEntry( i ).val );
        if( (i+1) % 16 == 0 || i == es - 1 ) printf( "\n" );
    }
    rcc0->level->on(); // modulate 1's
    rcc1->level->on(); // modulate 1's
    rcc0->getClock()->divider->set( 16 ); // 80/16=5 MHz 200 ns 
    rcc1->getClock()->divider->set( rcc0->getClock()->divider->get() ); // 16, to scale result properly
    rct->carrier->limited->on(); // data only
    rct->carrier->high->set( 32 ); // 12.5*32=400 ns
    rct->carrier->low->set( 96 ); // 12.5*96=1200 ns
    rct->carrier->enable->set( m );
    printf( "Carrier TX: M=%1d V=%1d L=%d H=%d\n", 
        rct->carrier->enable->get(), rcc0->level->get(),
        rct->carrier->low->get(), rct->carrier->high->get() );
    rcr->carrier->enable->set( m ); // when off for modulated, it sees every modulation pulse
    rcr->filter->enable->off();
    rcr->idle->set( 64 +1 ); // more than longest pulse on/off, in divided ticks
    rcr->carrier->low->set( 9 ); // 8-9; 7 - sees each pulse
    rcr->carrier->high->set( 10 ); // 10-99
    // 8.85 0=8031   1=0011   2=803f   3=0000 
    // 9.99 0=8032   1=0010   2=8040   3=0000 
    // 9.85 0=8032   1=0010   2=8040   3=0000
    //10.10 0=8039   1=0008   2=8000   3=0008   4=8000   5=0008
    //*9.10 0=8032   1=0010   2=8040   3=0000
    //*8.10 0=8031   1=0011   2=803f   3=0000
    // 7.10 0=8008   1=0003   2=8005   3=0003   4=8005   5=0003 ...  27=0000
    // 9.9  0=8038   1=000a   2=8000   3=000a   4=8000   5=000a
    rcr->carrier->remove->set( m );
    printf( "Carrier RX: M=%1d V=%1d L=%d H=%d I=%d\n", 
        rcr->carrier->enable->get(), rcc1->level->get(), 
        rcr->carrier->low->get(), rcr->carrier->high->get(), 
        rcr->idle->get() );
    rct->interrupt->enable->on();
    rct->interrupt->clear->on();
    printf( rct->interrupt, "TX type" ); 
    rcr->interrupt->enable->on();
    rcr->interrupt->clear->on();
    printf( rcr->interrupt, "RX type" ); 
    cnt->reset->on();
    cnt->reset->off();
    rcr->enable->on();
    rcc0->memory->ram->resetRead->on();
    rcc0->memory->ram->resetRead->off();
    rcc0->memory->fifo->reset->on();
    rcc0->memory->fifo->reset->off();
    rcc1->memory->ram->resetWrite->on();
    rcc1->memory->ram->resetWrite->off();
    rcc1->memory->fifo->reset->on();
    rcc1->memory->fifo->reset->off();
    rct->send.on->on();
    // generating - counting ...

    unsigned long pc = 0;
    while( ! rct->interrupt->masked->get() ) { pc++; } // 
    rct->send.on->off(); // already stopped, no rush
    printf( "Check count TX: %ld\n", pc ); 
    printf( rct->interrupt, "TX fired" ); 
    rct->interrupt->clear->on();
    printf( rct->interrupt, "TX cleared" ); 
    printf( "TX: Address  RAM: R=%d W=%d\n", rcc0->memory->ram->addressRead->get(), rcc0->memory->ram->addressWrite->get() ); 
    printf( "TX: Address FIFO: R=%d W=%d\n", rcc0->memory->fifo->addressRead->get(), rcc0->memory->fifo->addressWrite->get() ); 
    printf( pcu, "" );
    
    pc = 0;
    while( ! rcr->interrupt->masked->get() ) { pc++; } // no cycles cause slow printf's above
    printf( "Check count RX: %ld\n", pc ); 
    printf( rcr->interrupt, "RX fired" ); 
    rcr->interrupt->clear->on();
    printf( rcr->interrupt, "RX cleared" );
    printf( "RX: Address  RAM: R=%d W=%d\n", rcc1->memory->ram->addressRead->get(), rcc1->memory->ram->addressWrite->get() ); 
    printf( "RX: Address FIFO: R=%d W=%d\n", rcc1->memory->fifo->addressRead->get(), rcc1->memory->fifo->addressWrite->get() ); 
    
    rcc1->memory->setOwner( RemoteControlChannelMemory::Owner::SW_TRANSMITTER ); // otherwise trash on second+ run
    printf( "RX:  RAM:" );
    for( int i = 0; i < es; i++ )
    {
        printf( " %3d=%04hx", i, rcc1->memory->ram->getEntry( i ).val );
        if( (i+1) % 16 == 0 || i == es - 1 ) printf( "\n" );
    }

    rmt->memory->direct->off();
    rcc1->memory->fifo->reset->on();
    rcc1->memory->fifo->reset->off();
    printf( "RX: FIFO:" );
    for( int i = 0; i < es; i += 2 )
    {
        uint32_t v = rcc1->memory->fifo->data->get();
        printf( " %3d=%04hx", i, v & 0xFFFFu );
        printf( " %3d=%04hx", i+1, ( v >> 16 ) & 0xFFFFu );
        if( (i+1) % 16 == 0 || i == es - 1 ) printf( "\n" );
    }
    printf( "RX: Address FIFO: R=%d W=%d\n", rcc1->memory->fifo->addressRead->get(), rcc1->memory->fifo->addressWrite->get() ); 
    rmt->memory->direct->on();
    
    rcc1->memory->setOwner( RemoteControlChannelMemory::Owner::HW_RECEIVER ); // back to working mode
    rcr->interrupt->enable->off();
    rcr->enable->off();
    //TODO corrupts repeat() rcc0->getClock()->divider->set( 1 );
}

void repeat( RemoteControlChannel* const rcc0, PulseCountUnit::Counter* const cnt, uint32_t const delay )
{
    RemoteControlTransmitter* const rct = rcc0->getTransmitter();
    // next code for osc visualization
    cnt->pause->on(); // too much will be generated
    rct->continuous->on(); // repeats memory contents for this channel
    rcc0->memory->ram->resetRead->on();
    rcc0->memory->ram->resetRead->off();
    rct->send.on->on(); // required for both memory and FIFO
    // generating - counting ...
    vTaskDelay( delay );
    rct->send.off->on(); // forced stop
    rct->send.on->off();
    rct->send.off->off();
    rct->continuous->off(); // it doesn't stop alone
    cnt->pause->off();
}

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    // rmt_rx_start(0, 1);
    MicroControllerUnit* const mcu = new MicroControllerUnit();

    // Setup Remote Control, single channel

    RemoteControlController* rmt = mcu->periphery.getRemoteControlController();
    rmt->reset->off();
    rmt->clock->on();
    rmt->enable->on();
    rmt->memory->wrap->off();
    rmt->memory->direct->on();

    RemoteControlChannel* rcc0 = rmt->getChannel( 0 );
    rcc0->memory->setOwner( RemoteControlChannelMemory::Owner::SW_TRANSMITTER );
    rcc0->getClock()->setSource( RemoteControlClock::Source::APB );
    rcc0->getClock()->divider->set( 1 );

    RemoteControlTransmitter* rct = rcc0->getTransmitter();
    rct->send.on->off();
    rct->send.off->off();
    rct->idle->level->low();
    rct->idle->output->on();
    rct->interrupt->enable->off();
    rct->limit->interrupt->enable->off();
    rct->carrier->enable->off();

    RemoteControlChannel* rcc1 = rmt->getChannel( 1 );
    rcc1->memory->setOwner( RemoteControlChannelMemory::Owner::HW_RECEIVER );
    rcc1->getClock()->setSource( RemoteControlClock::Source::APB );
    rcc1->getClock()->divider->set( 1 );

    RemoteControlReceiver* rcr = rcc1->getReceiver();
    rcr->enable->off();
    rcr->carrier->enable->off();

    // Setup pulse counter (a test device)

    PulseCountController* pcnt = mcu->periphery.getPulseCountController();
    pcnt->reset->on(); // strangely it was needed once
    pcnt->reset->off();
    pcnt->clock->on();
    pcnt->enable->on();

    PulseCountUnit* pcu = pcnt->getUnit( 0 ); // 1..3 are good fit too
    pcu->getFilter()->enable->off(); // otherwise fast ticks will be lost

    PulseCountUnit::Counter* cnt = &pcu->counter;

    PulseCountComparator* pcuc = pcu->getComparator();
    pcuc->interrupt.enable->off();
    pcuc->interrupt.clear->on();
    pcuc->high.enable->off(); // no reset on limit
    pcuc->low.enable ->off(); // no reset on limit-
    
    PulseCountChannel* pcc0 = pcu->getChannel( 0 ); // all inputs in IGNORE
    pcc0->signal.setOnPositive( PulseCountChannel::Signal::Action::INCREASE );
    PulseCountChannel* pcc1 = pcu->getChannel( 1 ); // unused here and below, all inputs in IGNORE
        
    // Setup I/O

    ControllerIO* io = mcu->getControllerIO();
    printf( "I/O clock=%1d\n", io->clock->get() ); // found 1 on reset

    // Setup output for generator of pulses (remote control)

    ExternalPin* p4 = io->getExternalPin( PIN_OUT );
    p4->input->off();
    p4->function->set( FUN_OUT );
    p4->setPull( ExternalPin::Pull::OPEN );
    p4->setDriveStrength( ExternalPin::DriveStrength::OUT5MA ); // the more the better form
    p4->sleep.enable->off();

    // Setup input for receiver of pulses (pulse counter)

    ExternalPin* p8 = io->getExternalPin( PIN_INP );
    p8->input->on();
    p8->function->set( FUN_INP );
    p8->setPull( ExternalPin::Pull::OPEN );
    p8->filter->off();
    p8->sleep.enable->off();

    // Setup GPIO Matrix

    MatrixOutput* mo0 = io->getMatrixOutput( p4->number );
    mo0->periphery->set( rct->index );
    mo0->enable.setSource( MatrixOutput::Enable::Source::PERIPHERY );
    printf( mo0, p4 );

    MatrixInput* mi2 = io->getMatrixInput( rcr->index );
    mi2->matrix->on();
    mi2->gpio->set( p8->number );
    printf( mi2, p8 );

    MatrixInput* mi0s = io->getMatrixInput( pcc0->signal.index );
    mi0s->matrix->on();
    mi0s->gpio->set( p8->number );
    printf( mi0s, p8 );
    
    MatrixInput* mi0c = io->getMatrixInput( pcc0->control.index );
    mi0c->matrix->on(); // as pictured (Figure 5-1. IO MUX, RTC IO MUX and GPIO Matrix Overview); really it doesn't matter, on() or off()
    mi0c->gpio->set( MatrixInput::SOURCE_LOW );
    printf( mi0c, nullptr );
    
    MatrixInput* mi1s = io->getMatrixInput( pcc1->signal.index ); // not in use
    mi1s->matrix->on();
    mi1s->gpio->set( MatrixInput::SOURCE_LOW );
    printf( mi1s, nullptr );
    
    MatrixInput* mi1c = io->getMatrixInput( pcc1->control.index ); // not in use
    mi1c->matrix->on();
    mi1c->gpio->set( MatrixInput::SOURCE_LOW );
    printf( mi1c, nullptr );

    // Unit tests

    // 50+150=400kHz 5+15=4MHz APB=80MHz
    rmt_item16_t p( RemoteControlChannelMemory::entry(  true,  50 ) );
    rmt_item16_t n( RemoteControlChannelMemory::entry( false, 150 ) );
    rmt_item16_t e( RemoteControlChannelMemory::ENTRY_END );
    rmt_item16_t e0[4] // good for recognition
    { 
        RemoteControlChannelMemory::entry(  true, 48 ), // 200*48= 9600 ns,  9600/1600=6 pulses gen
        RemoteControlChannelMemory::entry( false, 16 ), // 200*16= 3200 ns,  3200/1600=2 pulses down
        RemoteControlChannelMemory::entry(  true, 64 ), // 200*64=12800 ns, 12800/1600=8 pulses gen
        // last zero is impossible to recognize in current setup
        RemoteControlChannelMemory::ENTRY_END,
    };
    const uint32_t s0 = sizeof(e0)/sizeof(rmt_item16_t);
    rmt_item16_t e1[5] // good for observation
    { 
        RemoteControlChannelMemory::entry(  true, 48 ), // 200*48= 9600 ns,  9600/1600=6 pulses gen
        RemoteControlChannelMemory::entry( false, 16 ), // 200*16= 3200 ns,  3200/1600=2 pulses down
        RemoteControlChannelMemory::entry(  true, 64 ), // 200*64=12800 ns, 12800/1600=8 pulses gen
        RemoteControlChannelMemory::entry( false, 40 ), // 200*40= 8000 ns,  8000/1600=5 pulses down
        // a zero above is impossible to recognize in current setup
        RemoteControlChannelMemory::ENTRY_END,
    };
    const uint32_t s1 = sizeof(e1)/sizeof(rmt_item16_t);

    uint32_t delay = 0;//1000;

    ram( rmt, rcc0, pcu, p, n ); if( delay ) repeat( rcc0, cnt, delay );
    fifo( rmt, rcc0, pcu, p, n ); if( delay ) repeat( rcc0, cnt, delay );
    continuous( rcc0, pcu, p, n );
    loop( rcc0, pcu, p, n, 100-1 );
    wrap( rmt, rcc0, pcu, p, n );
    frequency( rcc0, pcu, 1 ); if( delay ) repeat( rcc0, cnt, delay );
    frequency( rcc0, pcu, 2 ); if( delay ) repeat( rcc0, cnt, delay );
    frequency( rcc0, pcu, 3 ); if( delay ) repeat( rcc0, cnt, delay );
    frequency( rcc0, pcu, 4 ); if( delay ) repeat( rcc0, cnt, delay );
    frequency( rcc0, pcu, 5 ); if( delay ) repeat( rcc0, cnt, delay );
    frequency( rcc0, pcu, 10 ); if( delay ) repeat( rcc0, cnt, delay );
    modulator( rcc0, pcu, false, e0, s0 ); if( delay ) repeat( rcc0, cnt, delay );
    modulator( rcc0, pcu,  true, e0, s0 ); if( delay ) repeat( rcc0, cnt, delay );
    rcc0->getClock()->divider->set( 1 );
    recognition( rmt, rcc0, rcc1, pcu, false, e0, s0 ); //if( delay ) repeat( rcc0, cnt, delay );
    recognition( rmt, rcc0, rcc1, pcu,  true, e0, s0 ); //if( delay ) repeat( rcc0, cnt, delay );
    recognition( rmt, rcc0, rcc1, pcu, false, e1, s1 ); if( delay ) repeat( rcc0, cnt, delay ); // 200-500 ns/sq
    recognition( rmt, rcc0, rcc1, pcu,  true, e1, s1 ); if( delay ) repeat( rcc0, cnt, delay ); // 200 ns - mod, 
    rcc0->getClock()->divider->set( 1 );
    rcc1->getClock()->divider->set( 1 );

    // Done

    printf( "\n" );
    rmt->enable->off();
    rmt->clock->off();
    pcnt->enable->off();
    pcnt->clock->off();
    io->clock->off();
    
    delete mcu;
    printf( "Exit Xtensa program\n" );
}
