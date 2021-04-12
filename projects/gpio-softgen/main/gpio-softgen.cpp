/* GPIO Example: Software digital generator.

   The program turns signal on and off for digital output pin.

   !!! PIN_OUT must connect PIN_INP by wire to allow validation.

   Expected output:

Macro...   7996998 Hz
Inline 32 bit...   5452432 Hz
Simple 64 bit...   1211644 Hz
Write and read 64 bit...    754397 Hz
Test write and read 64 bit...    644897 Hz
Log write and read 64 bit...
                 0 v4=1 v8=1
                 ...
                49 v4=0 v8=0
       282 Hz
Channel 1 bit...   2695549 Hz
Channel write and read...   1310945 Hz
Test channel write and read...   1193556 Hz
Log channel write and read...
                 0 v4=1 v8=1
                 ...
                79 v4=0 v8=0
       271 Hz
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
constexpr double FREQ = 40000000.; // 40 MHz
constexpr uint32_t TIME_ATTEMPTS = 3;

void runRegMacro( const uint32_t max, SystemTimer* stm, const uint32_t m4 )
{
    printf( "Macro..." ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    uint32_t v4 = m4;
    // constexpr uint32_t RS = GPIO_OUT_W1TS_REG;
    // constexpr uint32_t RC = GPIO_OUT_W1TC_REG;
    // constexpr uint32_t R[2] { RC, RS };
    for( uint32_t i = 0; i < max; i++ )
    {
        REG_WRITE( GPIO_OUT_REG, v4 );
        v4 = ~v4 & m4;
        // next variants look better but result in same frequency
        // REG_WRITE( i & 1 ? RS : RC, m4 );
        // REG_WRITE( R[ i & 1 ], m4 );
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
void runInline32( const uint32_t max, SystemTimer* stm, MatrixBuffer* buf, const uint32_t m4 )
{
    printf( "Inline 32 bit..." ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    uint32_t v4 = m4;
    for( uint32_t i = 0; i < max; i++ )
    {
        buf->writeL( v4 );
        v4 = ~v4 & m4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
void runSimple( const uint32_t max, SystemTimer* stm, MatrixBuffer* buf, const uint64_t m4 )
{
    printf( "Simple 64 bit..." ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t v4 = m4;
    for( uint32_t i = 0; i < max; i++ )
    {
        buf->write( v4 );
        v4 = ~v4 & m4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
void runWrRead( const uint32_t max, SystemTimer* stm, MatrixBuffer* buf, const uint64_t m4, const uint64_t m8 )
{
    printf( "Write and read 64 bit..." ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t v4 = m4;
    for( uint32_t i = 0; i < max; i++ )
    {
        buf->write( v4 );
        // uint64_t v8 = buf->read() & m8;
        buf->read();
        v4 = ~v4 & m4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
void runVerified( const uint32_t max, SystemTimer* stm, MatrixBuffer* buf, const uint64_t m4, const uint64_t m8 )
{
    printf( "Test write and read 64 bit..." ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t v4 = m4;
    for( uint32_t i = 0; i < max; i++ )
    {
        buf->write( v4 );
        uint64_t v8 = buf->read() & m8;
        bool b4 = (bool) v4;
        bool b8 = (bool) v8;
        if( b4 != b8 )
            printf( "ERROR: %10d v4=%1d v8=%1d\n", i, b4, b8 );
        v4 = ~v4 & m4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
void runLogged( const uint32_t max, SystemTimer* stm, MatrixBuffer* buf, const uint64_t m4, const uint64_t m8 )
{
    printf( "Log write and read 64 bit...\n" ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t v4 = m4;
    for( uint32_t i = 0; i < max; i++ )
    {
        buf->write( v4 );
        uint64_t v8 = buf->read() & m8;
        printf( "\t%10d v4=%1d v8=%1d\n", i, (bool) v4, (bool) v8 );
        v4 = ~v4 & m4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
void runChannel( const uint32_t max, SystemTimer* stm, MatrixBuffer::Channel* ch4 )
{
    printf( "Channel 1 bit..." ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    bool b4 = true;
    for( uint32_t i = 0; i < max; i++ )
    {
        ch4->write( b4 );
        b4 = !b4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
void runChWrRead( const uint32_t max, SystemTimer* stm, MatrixBuffer::Channel* ch4, MatrixBuffer::Channel* ch8 )
{
    printf( "Channel write and read..." ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    bool b4 = true;
    for( uint32_t i = 0; i < max; i++ )
    {
        ch4->write( b4 );
        ch8->read();
        b4 = !b4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
void runChTested( const uint32_t max, SystemTimer* stm, MatrixBuffer::Channel* ch4, MatrixBuffer::Channel* ch8 )
{
    printf( "Test channel write and read..." ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
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
    
void runChLogged( const uint32_t max, SystemTimer* stm, MatrixBuffer::Channel* ch4, MatrixBuffer::Channel* ch8 )
{
    printf( "Log channel write and read...\n" ); fflush( stdout );
    vTaskDelay( 1 ); //esp_task_wdt_reset();
    uint64_t t0 = stm->getValidValue( TIME_ATTEMPTS );
    bool b4 = true;
    for( uint32_t i = 0; i < max; i++ )
    {
        ch4->write( b4 );
        bool b8 = ch8->read();
        printf( "\t%10d v4=%1d v8=%1d\n", i, b4, b8 );
        b4 = !b4;
    }
    uint64_t t1 = stm->getValidValue( TIME_ATTEMPTS );
    uint64_t t2 = stm->getValidValue( TIME_ATTEMPTS );
    printf( " %9.0f Hz\n", FREQ * max / ( t1 - t0 - (t2-t1) ) );
}
    
/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    ControllerIO* io = mcu->getControllerIO();
    MatrixBuffer* buf = io->buffer;
    MatrixBuffer::Channel* ch4 = buf->getChannel( PIN_OUT );
    MatrixBuffer::Channel* ch8 = buf->getChannel( PIN_INP );
    SystemTimer* stm = mcu->getSystemTimer();

    ExternalPin* p4 = io->getExternalPin( PIN_OUT );
    p4->input->off();
    p4->function->set( FUNC_GPIO4_GPIO4 );
    p4->setPull( ExternalPin::Pull::OPEN );
    p4->setDriveStrength( ExternalPin::DriveStrength::OUT5MA ); // the more the better form

    ExternalPin* p8 = io->getExternalPin( PIN_INP );
    p8->input->on();
    p8->function->set( FUNC_GPIO4_GPIO4 );
    //TODO ? p8->setPull( ExternalPin::Pull::UP );
    
    // esp_task_wdt_init( 100000u, false );
    // TaskHandle_t handle;
    // esp_task_wdt_add(handle);

    buf->setEnabled( BIT4 );
    runRegMacro( 50000000u, stm, BIT4 );            // 8000 kHz
    runInline32( 30000000u, stm, buf, BIT4 );       // 5450 kHz
    runSimple  (  8000000u, stm, buf, BIT4 );       // 1210 kHz
    runWrRead  (  5000000u, stm, buf, BIT4, BIT8 ); //  754 kHz
    runVerified(  5000000u, stm, buf, BIT4, BIT8 ); //  645 kHz
    runLogged  (       50u, stm, buf, BIT4, BIT8 ); //  280  Hz
    buf->setEnabled( 0uL );
    
    ch4->enable( true );
    ch8->enable( false );
    runChannel ( 16000000u, stm, ch4 );      // 2700 kHz
    runChWrRead(  8000000u, stm, ch4, ch8 ); // 1310 kHz
    runChTested(  8000000u, stm, ch4, ch8 ); // 1190 kHz
    runChLogged(       80u, stm, ch4, ch8 ); //  270  Hz
    ch4->enable( false );

    delete mcu;
    printf( "Exit Xtensa program\n" );
}
