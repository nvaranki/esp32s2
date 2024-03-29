/* ULP rcv Example: manual (software) start of processor.

   This file contains C code to run assembly program on the ULP. To emplloy C++ 
   API "mcu", the file itself is named as C++ file.

   The program wakes up an ULP rcv to run code on a request, while it is on hold. 
   To make sure the rcv really runs, an ASM program increments public variable.
   The program is loaded into slow memory with intentional offset.

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ulp_main.h"
#include "mcu.h"
#include "BinaryImageULP.hpp"

// ASM program placeholders
extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    CoprocessorULP* const ulp = mcu->getCoprocessorULP();
    CoreRISCV* const rcv = ulp->getCoreRISCV();
    TimerULP* const tmr = ulp->getTimerULP();
    
    // cancel next timer run if any
    tmr->active->set( false ); 
    
    // load code
    uint32_t offset32 = 128; // 128*4=512=0x00000200
    BinaryImageULP image( ulp_main_bin_start, ulp_main_bin_end - ulp_main_bin_start );
    esp_err_t e = image.loadAt( RTC_SLOW_MEM + offset32 );
    if( e != ESP_OK )
    {
        printf("Failed to load ULP program (%x), aborting...\n", e);
        delete mcu;
        return;
    }
    ulp->entry->set( &ulp_entry - RTC_SLOW_MEM + offset32 );
    
    // select rcv to run
    ulp->setConfig( CoprocessorULP::ConfigCore::CORE, static_cast<bool>( CoprocessorULP::Core::RISCV ) );
    // rcv->startOn->set( true );
    // rcv->clockOn->set( true );
    // rcv->clockOff->set( false );

    // init shared vars
    ulp_edge_count = 500; // not earlier than the program has been loaded 

    // go!
    // rcv->start->set( false );
    // rcv->start->set( true ); // 0->1 transition matters
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // single pass run should finish within delay
    printf("Edge count from ULP: %10d\n", ulp_edge_count);

    // rcv->start->set( false ); // it possibly can be applied right after "true"
    // rcv->clockOn->set( false );
    delete mcu;
    printf("Exit main program\n");
}
