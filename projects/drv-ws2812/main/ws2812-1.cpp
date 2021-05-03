/* WS2812 LED Example: Driver in use.

   The program uses WS2812 single LED driver to transit RGB values to the device.
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
#include "ws2812x1.h"
#include "mcu.h"

#define PIN_OUT 18 /* Saola-1 pin for WS2812 */

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    DriverWS2812* const drv = new DriverWS2812( mcu, 0, PIN_OUT );
    drv->getPin()->setPull( ExternalPin::Pull::OPEN ); // driver keeps idle state as 0
    int dRGB = 30;
    
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
    // drv->send( 0, 0, 0 );

    // full area, log
    for( int r = 1; r <= 256; r *= 2 )
    {
        for( int g = 1; g <= 256; g *= 2 )
        {
            for( int b = 1; b <= 256; b *= 2 )
            {
                drv->send( r - 1, g - 1, b - 1 );
                vTaskDelay( 5 );
            }
            // vTaskDelay( 1 );
        }   
        // vTaskDelay( 1 );
    }
    drv->send( 0, 0, 0 );

    drv->getController()->enable->off();
    drv->getController()->clock->off();
    delete drv;
    delete mcu;
    printf( "Exit Xtensa program\n" );
}
