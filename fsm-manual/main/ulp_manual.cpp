/* ULP FSM Example: manual (software) start of processor few times 
   with sync'd data transfer.

   This file contains C code to run assembly program on the ULP. To emplloy C++ 
   API "mcu", the file itself is named as C++ file.

   The program wakes up an ULP FSM to run code on a request, while it is on hold. 
   To make sure the FSM really runs, an ASM program increments public variable.

    Expected output:

Delay 10 task ticks
Edge count   0 from ULP:        501
Edge count   1 from ULP:    1000502
Edge count   2 from ULP:    2000503
Edge count   3 from ULP:    3000504
Edge count   4 from ULP:    4000505
Edge count   5 from ULP:    5000506
Exit main program

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ulp_main.h"
#include "mcu.h"
#include "BinaryImageULP.hpp"
#include "MicroControllerUnit.hpp"
#include "PowerManagementUnit.hpp"
#include "CoprocessorULP.hpp"
#include "CoreFSM.hpp"

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
    PowerManagementUnit* const pmu = mcu->getPowerManagementUnit();
    CoprocessorULP* const ulp = mcu->getCoprocessorULP();
    CoreFSM* const fsm = ulp->getCoreFSM();
    
    // load code
    BinaryImageULP image( ulp_main_bin_start, ulp_main_bin_end - ulp_main_bin_start );
    esp_err_t e = image.loadAt( RTC_SLOW_MEM );
    if( e != ESP_OK )
    {
        printf("Failed to load ULP program (%x), aborting...\n", e);
        delete mcu;
        return;
    }
    ulp->entry->set( &ulp_entry - RTC_SLOW_MEM );
    
    // select FSM to run
    fsm->selectForExec();
    fsm->startOn->on(); // manual (software) start
    fsm->clock->on->on();
    fsm->clock->off->off();

    // init shared vars
    ulp_edge_count = 500; // not earlier than the program has been loaded 

    // go!
    const TickType_t delay = 100 / portTICK_PERIOD_MS;
    printf("Delay %d task ticks\n", delay);
    fsm->start->off(); // get ready for first run
    for( int i = 0; i < 6; i++ ) 
    {
        fsm->start->on();
        do vTaskDelay( delay ); // let FSM go; single pass run should finish within delay
        while( !pmu->cocpuDone->get() );
        fsm->start->off(); // get ready for next run; it possibly can be applied right after "true"
        printf("Edge count %3d from ULP: %10d\n", i, ulp_edge_count);
        ulp_edge_count += 1000000;
    }

    // done
    fsm->clock->on->off();
    delete mcu;
    printf("Exit main program\n");
}
