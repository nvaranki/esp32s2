/* GPIO Example: Input signal to peripheral unit from GPIO pin.

   The program uses software digital generator to make digital signal.
   That, in turn, is received by pulse counter, working in different modes.
   Both direct GPIO input and GPIO Matrix are exploited.

   !!! PIN_OUT must connect PIN_INP by wire to allow proper function.

   Expected output (excerpt):

Unit 0: status=0x00 counter=+0 initially
Unit 0: status=0x00 counter=+100 INCREASE
Unit 0: status=0x00 counter=-100 DECREASE
Unit 0: status=0x00 counter=+0 IGNORE
Unit 0: status=0x00 counter=+200 INCREASE+ INCREASE-
Unit 0: status=0x00 counter=+0 BOUNCE
Unit 0: status=0x00 counter=+200
Unit 0: status=0x00 counter=+200 INHIBIT
Unit 0: status=0x00 counter=+200 INCREASE filtered on pin
Unit 0: status=0x00 counter=+200 INCREASE through matrix
Unit 0: status=0x00 counter=+200 INCREASE synch'd by APB clock raise and fall
Unit 0: status=0x00 counter=+2 INCREASE filtered by unit
Unit 0: status=0x00 counter=+200 INCREASE filtered on pin
Exit Xtensa program

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "soc/io_mux_reg.h"
#include "mcu.h"

#define PIN_OUT 4
#define PIN_INP 8
#define FUN_OUT FUNC_GPIO4_GPIO4 //TODO FUNC_GPIO4_GPIO4_0 ?
#define FUN_INP FUNC_GPIO8_GPIO8 //TODO FUNC_GPIO8_GPIO8_0 ?
constexpr double FREQ = 40000000.; // 2*40 MHz (two edges per pulse)
constexpr uint32_t TIME_ATTEMPTS = 3;

void runChannel( const uint32_t edges, MatrixBuffer::Channel* ch, SystemTimer* stm )
{
    printf( "Channel 1 bit generates " ); fflush( stdout );
    // vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    // MicroControllerUnit* const mcu = new MicroControllerUnit();
    // MatrixBuffer::Channel* ch8 = mcu->getControllerIO()->buffer->getChannel( PIN_INP );
    // printf( "\n\t" );
    for( uint32_t i = 1; i <= edges; i++ )
    {
        ch->write( i & 1 );
        // if( ( i & 127 ) == 0 ) printf( "\n\t" );
        // printf( "%1d", (bool) (i & 1) );
        // printf( "\t%10d v4=%1d\n", i, (bool) (i & 1) );
    //     ch->write( i & 1 );
    //     bool b8 = ch8->read();
    //     printf( "\t%10d v4=%1d v8=%1d\n", i, (bool) (i & 1), b8 );
    }
    // printf( "\n" );
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( "%d pulse edges at %.0f Hz\n", edges, FREQ * edges / ( t1 - t0 - ( t2 - t1 ) ) );
}

void runChTested( const uint32_t max, MatrixBuffer::Channel* ch4, MatrixBuffer::Channel* ch8, SystemTimer* stm )
{
    printf( "Test channel write and read..." ); fflush( stdout );
    // vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    bool b4 = true;
    for( uint32_t i = 0; i < max; i++ )
    {
        ch4->write( b4 );
        bool b8 = ch8->read();
        if( b4 != b8 )
            printf( "ERROR: %10d v4=%1d v8=%1d\n", i, b4, b8 );
        b4 = !b4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}

void printf( PulseCountChannel* pcc )
{
    printf( "Setup channel %d.%d: P=%d N=%d H=%d L=%d\n", pcc->unit, pcc->id, 
        (int) pcc->signal.getOnPositive(), (int) pcc->signal.getOnNegative(),
        (int) pcc->control.getOnHigh(),    (int) pcc->control.getOnLow() );
}

void printf( const PulseCountUnit* pcu, const char* comment )
{
    printf( "Unit %d: status=0x%02x counter=%+d %s\n", 
        pcu->id, (uint32_t) pcu->counter.getStatus(), pcu->counter.getValue(), comment );
}

void printf( const MatrixInput* mi, const ExternalPin* p )
{
    printf( "Peripheral input %d assigned to GPIO pin %02d %1s %s\n", 
        mi->input, mi->source->get(), p == nullptr ? "F" : p->input->get() ? "R" : "W",
        mi->matrix->get() ? "via matrix" : "directly" );
}

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    SystemTimer* stm = mcu->getSystemTimer();

    // Setup Pulse Counter, single channel of single unit

    PulseCountController* pcnt = mcu->periphery.getPulseCountController();
    pcnt->reset->on(); // strangely it was needed once
    pcnt->reset->off();
    pcnt->clock->on();
    pcnt->enable->on();

    PulseCountUnit* pcu = pcnt->getUnit( 0 ); // 1..3 are good fit too
    pcu->getFilter()->enable->off(); // otherwise fast ticks will be lost
    printf( pcu, "initially" );

    PulseCountComparator* pcuc = pcu->getComparator();
    pcuc->interrupt.enable->off();
    pcuc->interrupt.clear->on();
    pcuc->high.enable->off(); // no reset on limit
    // pcuc->high.threshold->set( pcuc->high.threshold->max() >> 1 ); // +32767
    pcuc->low.enable ->off(); // no reset on limit-
    // pcuc->low.threshold->set( ~( pcuc->high.threshold->max() >> 1 ) ); // -32768
    pcuc->thr0.enable->off();
    pcuc->thr1.enable->off();
    pcuc->zero.enable->off();
    
    PulseCountChannel* pcc0 = pcu->getChannel( 0 ); // all inputs in IGNORE
    printf( pcc0 );
    
    PulseCountChannel* pcc1 = pcu->getChannel( 1 ); // unused here and below, all inputs in IGNORE
    printf( pcc1 );
        
    // Setup I/O

    ControllerIO* io = mcu->getControllerIO();
    printf( "I/O clock=%1d\n", io->clock->get() ); // found 1 on reset

    // Setup output for generator of pulses

    ExternalPin* p4 = io->getExternalPin( PIN_OUT );
    p4->input->off();
    p4->function->set( FUN_OUT );
    p4->setPull( ExternalPin::Pull::OPEN );
    p4->setDriveStrength( ExternalPin::DriveStrength::OUT5MA ); // the more the better form
    p4->sleep.enable->off();

    MatrixBuffer::Channel* ch4 = io->buffer->getChannel( p4->number ); // direct 1 bit output to GPIO
    ch4->output( true );
    printf( "Output channel assigned to GPIO %02d %1s%1s\n", ch4->id, ch4->output() ? "W" : "R", p4->input->get() ? "R" : "W" );
    
    // Setup input for receiver of pulses (pulse counter)

    ExternalPin* p8 = io->getExternalPin( PIN_INP );
    p8->input->on();
    p8->function->set( FUN_INP );
    p8->setPull( ExternalPin::Pull::OPEN ); //TODO ? p8->setPull( ExternalPin::Pull::DOWN ); 
    p8->filter->off();
    p8->sleep.enable->off();

    // Setup GPIO Matrix

    MatrixInput* mi0s = io->getMatrixInput( pcc0->signal.index );
    mi0s->matrix->off();
    mi0s->source->set( p8->number );
    printf( mi0s, p8 );
    
    MatrixInput* mi0c = io->getMatrixInput( pcc0->control.index );
    mi0c->matrix->on(); // as pictured (Figure 5-1. IO MUX, RTC IO MUX and GPIO Matrix Overview); really it doesn't matter, on() or off()
    mi0c->source->set( MatrixInput::SOURCE_LOW );
    printf( mi0c, nullptr );
    
    MatrixInput* mi1s = io->getMatrixInput( pcc1->signal.index ); // not in use
    mi1s->matrix->on();
    mi1s->source->set( MatrixInput::SOURCE_LOW );
    printf( mi1s, nullptr );
    
    MatrixInput* mi1c = io->getMatrixInput( pcc1->control.index ); // not in use
    mi1c->matrix->on();
    mi1c->source->set( MatrixInput::SOURCE_LOW );
    printf( mi1c, nullptr );

    runChTested( 4u, ch4, io->buffer->getChannel( p8->number ), stm );

    // Unit tests

    printf( "\n" );
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    pcc0->signal.setOnPositive( PulseCountChannel::Signal::Action::INCREASE );
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "INCREASE" );

    printf( "\n" );
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    pcc0->signal.setOnPositive( PulseCountChannel::Signal::Action::DECREASE );
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "DECREASE" );
    
    printf( "\n" );
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    pcc0->signal.setOnPositive( PulseCountChannel::Signal::Action::IGNORE );
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "IGNORE" );
    
    printf( "\n" );
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    pcc0->signal.setOnPositive( PulseCountChannel::Signal::Action::INCREASE );
    pcc0->signal.setOnNegative( PulseCountChannel::Signal::Action::INCREASE );
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "INCREASE+ INCREASE-" );

    printf( "\n" );
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    runChannel ( 200u, ch4, stm );
    printf( pcu, "" );
    pcc0->control.setOnLow( PulseCountChannel::Control::Action::BOUNCE );
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "BOUNCE" );
    pcc0->control.setOnLow( PulseCountChannel::Control::Action::IGNORE );

    printf( "\n" );
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    runChannel ( 200u, ch4, stm );
    printf( pcu, "" );
    pcc0->control.setOnLow( PulseCountChannel::Control::Action::INHIBIT );
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "INHIBIT" );
    pcc0->control.setOnLow( PulseCountChannel::Control::Action::IGNORE );

    printf( "\n" );
    p8->filter->on();
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    pcc0->signal.setOnPositive( PulseCountChannel::Signal::Action::INCREASE );
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "INCREASE filtered on pin" );
    p8->filter->off();
    
    printf( "\n" );
    mi0s->matrix->on(); // via matrix
    printf( mi0s, p8 );
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    pcc0->signal.setOnPositive( PulseCountChannel::Signal::Action::INCREASE );
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "INCREASE through matrix" );
    
    printf( "\n" );
    p8->setSync( ExternalPin::SyncStage::FIRST,  ExternalPin::SyncMode::RAISE );
    p8->setSync( ExternalPin::SyncStage::SECOND, ExternalPin::SyncMode::FALL );
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "INCREASE synch'd by APB clock raise and fall" );
    
    printf( "\n" );
    pcu->getFilter()->enable->on();
    pcu->getFilter()->threshold->set( 100 ); // 10: counter=+200, 100: counter=+2 ?, 1000: counter=+0
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    printf( pcc0 );
    runChannel ( 400u, ch4, stm );
    printf( pcu, "INCREASE filtered by unit" );
    pcu->getFilter()->enable->off();
    p8->setSync( ExternalPin::SyncStage::FIRST,  ExternalPin::SyncMode::BYPASS );
    p8->setSync( ExternalPin::SyncStage::SECOND, ExternalPin::SyncMode::BYPASS );
    
    printf( "\n" );
    p8->filter->on();
    pcu->counter.reset->on();
    pcu->counter.reset->off();
    printf( pcc0 );
    runChannel ( 200u, ch4, stm );
    printf( pcu, "INCREASE filtered on pin" );
    p8->filter->off();
    
    printf( "\n" );
    pcnt->enable->off();
    pcnt->clock->off();
    io->clock->off();
    
    delete mcu;
    printf( "Exit Xtensa program\n" );
}
