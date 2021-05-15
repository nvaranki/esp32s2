/* AI performance test: Расчет ПТЭ на максимуме памяти.

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_task_wdt.h"
#include "mcu.h"
#include "DataBuilderFloat2D.hpp"


/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    SystemTimer* stm = mcu->getSystemTimer();
    printf( "Clock SoC: %d %1d %d\n", (uint32_t) mcu->getClockSoC(), mcu->pllFreqHigh->get(), mcu->upraiseClockSoC->get() );
    printf( "Heap size=%d\n", heap_caps_get_free_size( MALLOC_CAP_32BIT ) );
    
    DataBuilderFloat2D* db = new DataBuilderFloat2D( 5, 64, 64 ); // 5*64*64*2*4=163840 bytes
    printf( "Matrix data dimensions %ld*%d*%d*2*4=%ld bytes\n", 
            db->n, 
            db->sizeOut, 
            db->sizeInp, 
            db->n*db->sizeOut*db->sizeInp*2*4 );
    printf( "Heap size=%d\n", heap_caps_get_free_size( MALLOC_CAP_32BIT ) );
    
    uint64_t tX = stm->getValidValue( DataBuilderFloat2D::TIME_ATTEMPTS );
    //vTaskDelay( 100 );//( 1000 ); // t0-tX=799937342, * 25/2 = 10s as observed
    uint64_t t0 = stm->getValidValue( DataBuilderFloat2D::TIME_ATTEMPTS );
    uint64_t t32 = db->fill( stm );
    uint64_t t1 = stm->getValidValue( DataBuilderFloat2D::TIME_ATTEMPTS );
    // stm counts at 80 MHz --> 12.5 ns/tick
    printf( "Computed test data in %lld ns, %lld ns/matrix, %lld ns/layer, %lld ns/output D=%lld\n", 
            (t1-t0)*25/2, 
            t32*25/2, 
            t32*25/2/db->n, 
            t32*25/2/(db->n*db->sizeOut),
            (t0-tX)*25/2 );
    
    delete db;
    delete mcu;
    printf( "Exit Xtensa program\n" );
}
