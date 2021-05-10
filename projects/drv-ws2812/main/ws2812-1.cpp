/* WS2812 LED Example: Driver in use.

   The program uses WS2812 single/pair LED driver to transit RGB values to the device.
   That, in turn, employs hardware digital generator RMT to make a signal sequence
   supported by device. LED device emits submitted color.

   !!! PIN_OUT must match the design board.

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include <initializer_list>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "ws281X.h"
#include "mcu.h"

#define PIN_OUT 18 /* Saola-1 pin for WS2812 */

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();

    RemoteControlController* const rmt = mcu->periphery.getRemoteControlController();
    rmt->reset->off();
    rmt->clock->on();
    rmt->enable->on();
    rmt->memory->wrap->on(); // some tests can use 3+ LEDs

//  DriverWS2812* const drv = new DriverWS2812( mcu, 0, PIN_OUT ); // original Saola-1
    DriverSK6812MINI* const drv = new DriverSK6812MINI( mcu, 0, PIN_OUT ); // clone of Saola-1
    drv->pin->setPull( ExternalPin::Pull::OPEN ); // driver keeps idle state as 0, LED pulls up
    
    int dRGB = 30, dBlock = 50, dCycle = 5;

    drv->send( 0, 0, 0 ); // fix: occasionally fires white after flash rewrite or system reset
    
    // only red
    drv->send( 0xFFu, 0, 0 ); vTaskDelay( dRGB );
    drv->send( 0xF0u, 0, 0 ); vTaskDelay( dRGB );
    drv->send( 0x0Fu, 0, 0 ); vTaskDelay( dRGB );
    drv->send( 0x00u, 0, 0 ); vTaskDelay( dRGB );
    
    // only green
    drv->send( 0, 0xFFu, 0 ); vTaskDelay( dRGB );
    drv->send( 0, 0xF0u, 0 ); vTaskDelay( dRGB );
    drv->send( 0, 0x0Fu, 0 ); vTaskDelay( dRGB );
    drv->send( 0, 0x00u, 0 ); vTaskDelay( dRGB );
    
    // only blue
    drv->send( 0, 0, 0xFFu ); vTaskDelay( dRGB );
    drv->send( 0, 0, 0xF0u ); vTaskDelay( dRGB );
    drv->send( 0, 0, 0x0Fu ); vTaskDelay( dRGB );
    drv->send( 0, 0, 0x00u ); vTaskDelay( dRGB );

    // long sequence in wrap mode
    const int cc = 500;
    union 
    {
        /** identified colors */
        DriverWS2812::Color color;
        /** raw data */
        uint8_t triple[3];
    } 
    led[cc];
    for( int d = 0; d < cc; d++ )    
        for( int i = 0; i < 3; i++ ) 
            led[d].triple[i] = d == 0 ? 0x40 : ( 0x10 * d + i ) & 0xFF; // only first triple can be seen
    drv->send( &led->color, cc );
    vTaskDelay( dBlock );

    // full area, linear
    // uint8_t s = 4; // step 1 takes very long time (22+ minutes)
    // for( int r = 0; r <= UINT8_MAX; r += s )
    // {
    //     for( int g = 0; g <= UINT8_MAX; g += s )
    //     {
    //         for( int b = 0; b <= UINT8_MAX; b += s )
    //         {
    //             drv->send( r, g, b );
    //             // vTaskDelay( 1 );
    //         }
    //         // vTaskDelay( 1 );
    //     }   
    //     vTaskDelay( 1 );
    // }
    // vTaskDelay( dBlock );

    // full area, log
    int f = 4;//2;
    for( int r = 1; r <= 256; r *= f )
    {
        for( int g = 1; g <= 256; g *= f )
        {
            for( int b = 1; b <= 256; b *= f )
            {
                drv->send( r - 1, g - 1, b - 1 );
                vTaskDelay( dCycle );
            }
            // vTaskDelay( 1 );
        }   
        // vTaskDelay( 1 );
    }
    vTaskDelay( dBlock );
    
    // two LEDs, second is virtual 
    drv->send( 0, 0, 0, 0xFFu, 0xFFu, 0xFFu ); 
    vTaskDelay( dBlock );

    delete drv;
    delete mcu;
    printf( "Exit Xtensa program\n" );
}
