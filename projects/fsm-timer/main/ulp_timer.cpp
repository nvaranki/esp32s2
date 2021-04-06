/* ULP FSM Example: timed (hardware) start of processor and async data exchange.

   This file contains C code to run assembly program on the ULP. To emplloy C++ 
   API "mcu", the file itself is named as C++ file.

   The program prepares ULP timer that periodically wakes up an ULP FSM to run code.
   Main program stays on hold to let FSM to do some work few times. 
   To make sure the FSM really runs, an ASM program increments public variable.

    Expected output follows. Because of async data retreival, low numbers vary by +-1.

Delay 100 task ticks
Edge count   0 from ULP:        505
Edge count   1 from ULP:    1000510
Edge count   2 from ULP:    2000515
Edge count   3 from ULP:    3000520
Edge count   4 from ULP:    4000524
Edge count   5 from ULP:    5000529
Stop ULP tomer and FSM
Exit main program

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
    CoreFSM* const fsm = ulp->getCoreFSM();
    TimerULP* const tmr = ulp->getTimerULP();
    
    // cancel next timer run if any
    tmr->active->off(); 
    
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

    // init shared vars
    ulp_edge_count = 500; // not earlier than the program has been loaded 
    
    // select FSM to run
    fsm->selectForExec();
    fsm->selectForDone(); // triggers ULP timer on HALT
    fsm->startOn->off(); // block software start
    fsm->clock->on->on();
    fsm->clock->off->off();

    // start timer
    tmr->sleep->set( 20000 );
    tmr->active->on();

    // go!
    const TickType_t delay = 1000 / portTICK_PERIOD_MS;
    printf("Delay %d task ticks\n", delay);
    for( int i = 0; i < 6; i++ ) 
    {
        vTaskDelay( delay ); // let FSM and timer pass some amount of cycles
        printf("Edge count %3d from ULP: %10d\n", i, ulp_edge_count);
        ulp_edge_count += 1000000;
    }

    // done
    printf("Stop ULP timer and FSM\n");
    tmr->active->off();
    fsm->clock->on->off();
    delete mcu;
    printf("Exit main program\n");
}
