/* ULP FSM Example: timed (hardware) start of processor.

   This file contains C code to run assembly program on the ULP. To emplloy C++ 
   API "mcu", the file itself is named as C++ file.

   The program prepares ULP timer that periodically wakes up an ULP FSM to run code.
   Main program stays on hold to let FSM to do some work few times. 
   To make sure the FSM really runs, an ASM program increments public variable.

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp32/ulp.h"
#include "ulp_main.h"
#include "mcu.h"

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
    tmr->active->set( false ); 
    
    // load code
    if( ulp->loadExecCode( 0, ulp_main_bin_start,
        (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t) ) != ESP_OK )
    {
        printf("Failed to load ULP program, aborting...\n");
        delete mcu;
        return;
    }
    ulp->setEntryPoint( &ulp_entry - RTC_SLOW_MEM );
    
    // select FSM to run
    ulp->setConfig( CoprocessorULP::ConfigCore::CORE, static_cast<bool>( CoprocessorULP::Core::FSM ) );
    ulp->setConfig( CoprocessorULP::ConfigCore::DONE, 0/*static_cast<bool>( CoprocessorULP::Core::FSM )*/ ); // in: ULP trigger source
    fsm->startOn->set( false ); // block software start
    fsm->clockOn->set( true );
    fsm->clockOff->set( false );

    // init shared vars
    ulp_edge_count = 500; // not earlier than the program has been loaded 

    // start timer
    tmr->sleep->set( 2000 );
    tmr->active->set( true );

    // go!
    const TickType_t delay = 1000 / portTICK_PERIOD_MS;
    printf("Delay %d task ticks\n", delay);
    for( int i = 0; i < 6; i++ ) 
    {
        vTaskDelay( delay );
        printf("Edge count %3d from ULP: %10d\n", i, ulp_edge_count);
        ulp_edge_count += 1000000;
    }

    tmr->active->set( false );
    fsm->clockOn->set( false );
    delete mcu;
    printf("Exit main program\n");
}
